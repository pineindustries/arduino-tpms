test1234

/* Community Libraries */
#include <SPI.h>

/* Custom Libraries */
#include "tpms/waveshare.h"
#include "tpms/cc1101.h"

/* Microcontroller SPI Interface Pins 
 * Shared between the LCD and the CC1101
*/
#define SCK_PIN   13  
#define MISO_PIN  12
#define MOSI_PIN  11

/* LCD Pin */
#define LCD_CS_PIN 7

/* CC1101 Pins and Registers */
#define CC1101_CS_PIN      10
#define CC1101_GDO0_PIN     2
#define SIDLE            0x36 // Set IDLE Mode
#define SRX              0x34 // Set Rx Mode
#define SRES             0x30 // Reset Chip

/*  Sensor Status Matrix 
 *    Columns correspond to grid_num
 *      0 --> Test
 *      1 --> Front Driver
 *      2 --> Front Passenger
 *      3 --> Back Driver
 *      4 --> Back Passenger
*/
volatile byte status[3][5] = {
                               { 0, 0, 0, 0, 0 },  // New Update, 0 (No) or 1 (Yes)
                               { 0, 0, 0, 0, 0 },  // Pressure
                               { 0, 0, 0, 0, 0 }   // Temperature
                             };

/* Time at which LCD last drew individual sensor values on LCD */
unsigned long last_update[5] = {0};

/** For Testing
 *   When using a test sensor, idx cyclically iterates
 *   from grid_num = 1 through grid_num = 4
*/
byte idx = 0; 

/** byte2psi()
 *  
 * Input:
 *   byte b: 8-bit raw pressure value read from sensor
 *   
 * Returns (double):
 *   Raw pressure value converted to psi
*/
double byte2psi( byte b ) {

  if (!b) { return 0; }
  
  double psi = (double)b / 4;
  
  psi = -0.5 
          + (byte)(psi + 0.75) * 0.3 
            + (byte)( (b+1) % 2 + 2*psi + 0.75 ) * 0.4 
              + (byte)(psi + 0.25) * 0.5;
              
  return psi;
  
}

void setRx() {
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE3));
  digitalWrite(CSN_PIN, LOW);
  while( digitalRead(MISO_PIN) );
  SPI.transfer(SRX);
  digitalWrite(CSN_PIN, HIGH);
  SPI.endTransaction();
}

/** syncFound()
 *  
 *  Triggers when digital pin 2 falls, and 
 *  extracts TPMS packet contents as received on the CC1101
*/
void syncFound(void) {

  //Serial.println("sf");
  byte fifo_length, grid_num;
  
  /* TPMS Packet Contents */
  byte buffer[13] = {0};
  
  /* Check for Zero-Length FIFO */
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE3));
  digitalWrite(CC1101_CS_PIN, LOW);
  while( digitalRead(MISO_PIN) );
  SPI.transfer( 0xFB );
  fifo_length = SPI.transfer(0);
  digitalWrite(CC1101_CS_PIN, HIGH);
  SPI.endTransaction();
  
  if (!fifo_length) { 
    setRx();
    return; 
  }
	
	/* Get RXFIFO_OVERFLOW Bit */
	Serial.println(".");

  /* Read Burst FIFO to buffer */
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE3));
  digitalWrite(CC1101_CS_PIN, LOW);
  while( digitalRead(MISO_PIN) );
  SPI.transfer( 0xFF );  // 0x3F | 0xC0

    buffer[0]  = SPI.transfer(0);
    buffer[1]  = SPI.transfer(0);
    buffer[2]  = SPI.transfer(0);
    buffer[3]  = SPI.transfer(0);
    buffer[4]  = SPI.transfer(0);
    buffer[5]  = SPI.transfer(0);
    buffer[6]  = SPI.transfer(0);
    buffer[7]  = SPI.transfer(0);
    buffer[8]  = SPI.transfer(0);
    buffer[9]  = SPI.transfer(0);
    buffer[10] = SPI.transfer(0);
    buffer[11] = SPI.transfer(0);
    buffer[12] = SPI.transfer(0);

  digitalWrite(CC1101_CS_PIN, HIGH);
  SPI.endTransaction();
  
  /* Check Sensor ID */
  /* Your code may vary based on your sensor IDs */
  if ( buffer[7] != 0xfd && buffer[7] != 0xfd ) {
    //Serial.println("  x Unknown Sensor [6/7]");
    setRx();
    return;
  }
  if ( buffer[8] == 0xc8 && buffer[9] == 0x48 )      { grid_num = 0; }
  else if ( buffer[8] == 0xc0 && buffer[9] == 0x56 ) { grid_num = 1; }
  else if ( buffer[8] == 0x6b && buffer[9] == 0x79 ) { grid_num = 2; }
  else if ( buffer[8] == 0x66 && buffer[9] == 0xd0 ) { grid_num = 3; }
  else if ( buffer[8] == 0xbd && buffer[9] == 0x7b ) { grid_num = 4; }
  else {  
         //Serial.println("  x Unknown Sensor"); 
         //SpiStrobe(SRX);
         setRx();
         return; 
       }

  /* No New Updates */
  if ( status[0][grid_num] ) { 
    //Serial.println("  x No new updates."); 
    //SpiStrobe(SRX);
    setRx();
    return; 
  }
   
  /* Verify Checksum */
  if ( buffer[12] != (
                       (
                         buffer[0]
                         + buffer[1]
                         + buffer[2]
                         + buffer[3]
                         + buffer[4]
                         + buffer[5]
                         + buffer[6]
                         + buffer[7]
                         + buffer[8]
                         + buffer[9]
                         + buffer[10]
                         + buffer[11]
                       )
                       & 0xFF
                     )
     ) {
    //Serial.println("  x Checksum Fail.");
    //SpiStrobe(SRX);
    setRx();
    return;
  }

  /* No New Updates (Unless Testing) */
  if ( grid_num != 0 ) {
    if ( buffer[10] == status[1][grid_num] && buffer[11] == status[2][grid_num] ) {
      //Serial.println("  x New packets same as the old.");
      status[0][grid_num] = 0;
      //SpiStrobe(SRX);
      setRx();
      return;
    }
  }
  
  /* Store Packet Data for Processing */
  status[0][grid_num] = 1;
  status[1][grid_num] = buffer[10];
  status[2][grid_num] = buffer[11];
  //Serial.print(".");

  /* Return to Rx Mode */
  //SpiStrobe(SRX);
  setRx();

}

/** setup()
 *  
 *   Initializes peripherals and prepares for packet reception
*/
void setup() {

  /* Begin SPI */
  SPI.begin();

  /* Initialize Peripherals */
  initCC1101();
  initDisplay();
  
  /* Wait for Peripherals */
  delay(7000);
	
	Serial.begin(9600);
  while (!Serial) { delay(1); }
  Serial.println("Ready!");

  /* Ready to Receive Packets */
  SPI.usingInterrupt(digitalPinToInterrupt(CC1101_GDO0_PIN));
  attachInterrupt(
                   digitalPinToInterrupt(CC1101_GDO0_PIN), 
                   syncFound, 
                   FALLING
                 );
  
  /* Enable Receive Mode on CC1101 */
  SpiStrobe(SRX);
  
}

/** loop()
 *  
 *   Iterates through status[0][i] checking for updates and
 *   if updates are available, that tire's pressure and temperature
 *   are displayed on the LCD
*/
void loop() {

  byte i = 5;
  double px, temp;
  unsigned long m,d;
    
  while ( i-- ) {
    
    if ( status[0][i] ) {

      m = millis();
      d = m - last_update[i];

      if ( d < 7000 ) {
        //Serial.println("x");
        status[0][i] = 0;
        continue;
      }

      if ( d >= 7000 ) {

        SpiStrobe(SIDLE);

        px   = byte2psi( status[1][i] );
        temp = (( (double)status[2][i] - 60 ) * 9 / 5 + 32);
          
        if ( i == 0 ) {
          //Serial.println(".");
          displayTPMS( (idx++ % 4) + 1, 32.5, 100 );                             
        } else {
          displayTPMS( i, px, temp );                             
        }

        last_update[i] = m;
        status[0][i]   = 0;

        SpiStrobe(SRX);
        
      }
    }
  }

}
