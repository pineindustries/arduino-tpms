// Arduino Headers
#include <SPI.h>

// Custom Headers
#include "tpms/config.h"
#include "tpms/waveshare.h"
#include "tpms/cc1101.h"

// CC1101 Registers
#define SIDLE   0x36 // Set IDLE Mode
#define SRX     0x34 // Set Rx Mode
#define SRES    0x30 // Reset Chip
#define RXBYTES 0xFB // Number of Bytes in RX FIFO

/*  Sensor Status Matrix 
 *    Columns correspond to grid_num
 *      0 --> Test
 *      1 --> Front Driver
 *      2 --> Front Passenger
 *      3 --> Back Driver
 *      4 --> Back Passenger
*/
volatile uint8_t status[3][5] = {
                                  { 0, 0, 0, 0, 0 },  // New Update, 0 (No) or 1 (Yes)
                                  { 0, 0, 0, 0, 0 },  // Pressure
                                  { 0, 0, 0, 0, 0 }   // Temperature
                                };

/** byte2psi()
 *  
 * Input:
 *   uint8_t b: 8-bit raw pressure value read from sensor
 *   
 * Returns (double):
 *   Raw pressure value converted to psi
*/
float byte2psi( uint8_t b ) {

  if (!b) { return 0; }
  
  float psi = (float)b / 4;
  
  psi = -0.5 
          + (uint8_t)(psi + 0.75) * 0.3 
            + (uint8_t)( (b+1) % 2 + 2*psi + 0.75 ) * 0.4 
              + (uint8_t)(psi + 0.25) * 0.5;
              
  return psi;
  
}

/** syncFound()
 *  
 *  Interrupt Function: Triggers when PIN_GDO0 falls
*/
void syncFound(void) {

  // Check for Zero-Length FIFO
  if (!SpiReadReg(RXBYTES)) {
    SpiStrobe(SRX);
    return; 
  }

  // TPMS Packet Contents
  uint8_t buffer[13] = {0};

  // Burst Read the RX FIFO into buffer
  SpiBurstReadReg(0xFF, buffer, 13);

  // Check Sensor ID and Get grid_num
  if ( buffer[6] != 0x80 || buffer[7] != 0xfd ) {
    SpiStrobe(SRX);
    return;
  }

  bool s[5] = {
    buffer[8] == 0xc8 && buffer[9] == 0x48,  // Test
    buffer[8] == 0xc0 && buffer[9] == 0x56,  // Front Driver
    buffer[8] == 0x6b && buffer[9] == 0x79,  // Front Passenger
    buffer[8] == 0x66 && buffer[9] == 0xd0,  // Rear Driver
    buffer[8] == 0xbd && buffer[9] == 0x7b   // Rear Passenger
  };

  if ( !s[0] && !s[1] && !s[2] && !s[3] && !s[4] ) { 
    SpiStrobe(SRX);
    return;
  }

  uint8_t grid_num = 1 * s[1] + 2 * s[2] + 3 * s[3] + 4 * s[4];

  // Need to Draw Prior Update (Probably Receiving a Burst of Packets Right Now)
  if (status[0][grid_num]) { 
    SpiStrobe(SRX);
    return; 
  }
  
  // Verify Checksum
  if ( buffer[12] != (
                       (
                         buffer[3]
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
    SpiStrobe(SRX);
    return;
  }

  // Update Status Matrix
  status[0][grid_num] = grid_num == 0 || ( buffer[10] != status[1][grid_num] || buffer[11] != status[2][grid_num] );
  status[1][grid_num] = buffer[10];
  status[2][grid_num] = buffer[11];

  // Return to Rx Mode
  SpiStrobe(SRX);

}

/** setup()
 *  
 *   Initializes peripherals and prepares for packet reception
*/
void setup() {

  // Begin SPI
  SPI.begin();
  SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE3));

  // Initialize Peripherals
  initCC1101();
  initDisplay();

  // Attach Interrupt to PIN_GDO0
  SPI.usingInterrupt(digitalPinToInterrupt(PIN_GDO0));
  attachInterrupt(
                   digitalPinToInterrupt(PIN_GDO0), 
                   syncFound, 
                   FALLING
                 );
  
  // Enable Receive Mode on CC1101
  SpiStrobe(SRX);
  
}

/** loop()
 *  
 *   Iterates through status[0][i] checking for updates and
 *   if updates are available, that tire's pressure and temperature
 *   are displayed on the LCD
*/

// Test Sensor Cycles through grid_num = 1 through 4
uint8_t idx = 0;

// Time matrix indicating when we last drew sensor grid_num on LCD
unsigned long last_update[5] = {0};

void loop() {

  uint8_t i = 5;
  float px, temp;
  unsigned long m;
  
  // Test Sensor Cycles through grid_num = 1 through 4
  static uint8_t idx = 0;

  while ( i-- ) {
    
    if ( status[0][i] ) {     

      // Time Metrics
      m = millis();

      // Avoid Displaying the Same Packet During a Burst
      if ( millis() - last_update[i] < 3000 ) {
        status[0][i] = 0;
        continue;
      }

      // Prevent Race Conditions on the SPI Bus
      noInterrupts();

      // Set IDLE
      SpiStrobe(SIDLE);

      // Raw Data to Human-Readable
      px = byte2psi( status[1][i] );
      temp = ( (float)status[2][i] - 60 ) * 9 / 5 + 32;
        
      // Draw Values on LCD
      if ( i == 0 ) {
        displayTPMS( (idx++ % 4) + 1, 32.5, 100 );                             
      } else {
        displayTPMS( i, px, temp );                             
      }

      // Update Metrics
      last_update[i] = m;
      status[0][i] = 0;

      // Back to Listening
      SpiStrobe(SRX);

      // Re-Enable Interrupts
      interrupts();

    }
  }
}