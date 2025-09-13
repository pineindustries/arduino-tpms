#include "cc1101.h"

void SpiStrobe(byte strobe) {

  SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE3));
  digitalWrite(PIN_CS0, LOW);
  while( digitalRead(PIN_MISO) );
  
  SPI.transfer(strobe);
  
  digitalWrite(PIN_CS0, HIGH);
  SPI.endTransaction();
  
}

void SpiWriteReg(byte addr, byte value) {
	
  SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE3));
  digitalWrite(PIN_CS0, LOW);
  while( digitalRead(PIN_MISO) );
  
  SPI.transfer(addr);
  delay(1);
  SPI.transfer(value); 
  
  digitalWrite(PIN_CS0, HIGH);
  SPI.endTransaction();
  
}

byte SpiReadReg(byte addr) {
	
	byte value;
	
  SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE3));
  digitalWrite(PIN_CS0, LOW);
  while( digitalRead(PIN_MISO) );
  
  SPI.transfer(addr | 0x80);
  delay(1);
  value = SPI.transfer(0); 
  
  digitalWrite(PIN_CS0, HIGH);
  SPI.endTransaction();
	
	return value;
  
}

void initCC1101(void) {

  /* Set to IDLE */
  SpiStrobe(SIDLE);
  
  /* Begin CC1101 Register Configuration */
  SpiWriteReg(IOCFG0,   0x06);  // GDO0: HIGH if Sync Word is Found
  
  /* Packet Control */
  SpiWriteReg(SYNC1,    0x00);  // Sync Word High Byte
  SpiWriteReg(SYNC0,    0x00);  // Sync Word Low Byte
  SpiWriteReg(PKTLEN,   0x0D);  // Packet Length in Bytes
  SpiWriteReg(PKTCTRL1, 0x00);  // PQT OFF; CRC, Status Append, & Address Checks OFF
  SpiWriteReg(PKTCTRL0, 0x00);  // CRC & Whitening OFF; Fixed Packet Length ON
  SpiWriteReg(ADDR,     0x00);  // Disabled (See PCKTCTRL1)
  SpiWriteReg(CHANNR,   0x00);  // Channel Spacing Disabled

  /* Modem Configuration */
  SpiWriteReg(MDMCFG4,  0xD8);  // Data Rate and Receiver Bandwidth: 8.15 and 81.25 kHz
  SpiWriteReg(MDMCFG3,  0x49);  // Data Rate and Receiver Bandwidth: 8.15 and 81.25 kHz
  SpiWriteReg(MDMCFG2,  0x3A);  // DC Blocking OFF; Set ASK/OOK; Manchester ON;  Sync Mode: 16/16
  SpiWriteReg(MDMCFG1,  0x00);  // FEC OFF; TX Preamble: 2 bytes; Channel Spacing Exponent = 0
  SpiWriteReg(MDMCFG0,  0x00);  // Channel Spacing Mantissa = 0

  /* Automatic Gain Control (DN022) */
  SpiWriteReg(AGCCTRL2, 0x07);  // Averaged Amplitude of the Digital Filter (Per DN022: 0x03 through 0x07)
  SpiWriteReg(AGCCTRL1, 0x00);  // Set per DN022 for ASK/OOK
  SpiWriteReg(AGCCTRL0, 0x91);  // ASK/OOK Decision Boundary: 4dB (Per DN022 set 0x91 or 0x92)

  /* Receive Frequency: 433.919830 MHz */
  SpiWriteReg(FSCTRL1,  0x06);  // Intermediate Frequency (Set by SmartRF Studio, See DN022)
  SpiWriteReg(FSCTRL0,  0x00);  // Frequency Synthesizer Control Not Supported for ASK
  SpiWriteReg(FREQ2,    0x10);  // High Byte of Rx Frequency
  SpiWriteReg(FREQ1,    0xB0);  // Middle Byte of Rx Frequency
  SpiWriteReg(FREQ0,    0x71);  // Low Byte of Rx Frequency
	
  /* Filter Bandwidth (See DN022) */
  SpiWriteReg(FREND1,   0x56);  // Commensurate with 83 kHz Receive Bandwidth
  SpiWriteReg(MCSM1,    0x20);  // CCA Unless Receiving a Packet; Go IDLE after TX or Packet RX
  SpiWriteReg(MCSM0,    0x18);  // Manual Calibration; XOSC OFF during SLEEP mode
  SpiWriteReg(FOCCFG,   0x00);  // Not Supported for ASK (Always use 0)
  SpiWriteReg(BSCFG,    0x01);  // SmartRF Studio Defaults
	
  /* Other */
  SpiWriteReg(WOREVT1,  0x87);  // SmartRF Studio Defaults (1.0 s Timeout with 26.0 MHz Crystal)
  SpiWriteReg(WOREVT0,  0x6B);  // SmartRF Studio Defaults (1.0 s Timeout with 26.0 MHz Crystal)
  SpiWriteReg(WORECTRL, 0x03);  // SmartRF Studio Defaults
  SpiWriteReg(FSCAL3,   0xE9);  // SmartRF Studio Defaults
  SpiWriteReg(FSCAL2,   0x2A);  // SmartRF Studio Defaults
  SpiWriteReg(FSCAL1,   0x00);  // SmartRF Studio Defaults
  SpiWriteReg(FSCAL0,   0x1F);  // SmartRF Studio Defaults
  SpiWriteReg(RCCTRL1,  0x41);  // SmartRF Studio Defaults
  SpiWriteReg(RCCTRL0,  0x00);  // SmartRF Studio Defaults
  SpiWriteReg(TEST0,    0x09);  // SmartRF Studio Defaults

  /* Flush the RX FIFO */
	SpiStrobe(SFRX);
	
}
