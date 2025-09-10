// Community Includes
#include <SPI.h>

// SPI Configuration
#define SCK_PIN      13
#define MISO_PIN     12
#define MOSI_PIN     11
#define CSN_PIN      10
#define GDO0_PIN     2
#define SPI_SPEED    20000000

// CC1101 Configuration Registers
#define IOCFG2       0x00    // GDO2 output pin configuration
#define IOCFG1       0x01    // GDO1 output pin configuration
#define IOCFG0       0x02    // GDO0 output pin configuration
#define FIFOTHR      0x03    // RX FIFO and RX FIFO Thresholds
#define SYNC1        0x04    // Sync word, high INT8U
#define SYNC0        0x05    // Sync word, low INT8U
#define PKTLEN       0x06    // Packet length-
#define PKTCTRL1     0x07    // Packet automation control
#define PKTCTRL0     0x08    // Packet automation control
#define ADDR         0x09    // Device address
#define CHANNR       0x0A    // Channel number
#define FSCTRL1      0x0B    // Frequency Synthesizer control
#define FSCTRL0      0x0C    // Frequency Synthesizer control
#define FREQ2        0x0D    // Frequency control word, high INT8U
#define FREQ1        0x0E    // Frequency control word, middle INT8U
#define FREQ0        0x0F    // Frequency control word, low INT8U
#define MDMCFG4      0x10    // Modem configuration
#define MDMCFG3      0x11    // Modem configuration
#define MDMCFG2      0x12    // Modem configuration
#define MDMCFG1      0x13    // Modem configuration
#define MDMCFG0      0x14    // Modem configuration
#define DEVIATN      0x15    // Modem deviation setting
#define MCSM2        0x16    // Main Radio Control State Machine configuration
#define MCSM1        0x17    // Main Radio Control State Machine configuration
#define MCSM0        0x18    // Main Radio Control State Machine configuration
#define FOCCFG       0x19    // Frequency Offset Compensation configuration
#define BSCFG        0x1A    // Bit Synchronization configuration
#define AGCCTRL2     0x1B    // AGC control
#define AGCCTRL1     0x1C    // AGC control
#define AGCCTRL0     0x1D    // AGC control
#define WOREVT1      0x1E    // High byte Event 0 timeout
#define WOREVT0      0x1F    // Low byte Event 0 timeout
#define WORECTRL     0x20    // Wake On Radio control
#define FREND1       0x21    // Front end RX configuration
#define FREND0       0x22    // Front end RX configuration
#define FSCAL3       0x23    // Frequency Synthesizer calibration
#define FSCAL2       0x24    // Frequency Synthesizer calibration
#define FSCAL1       0x25    // Frequency Synthesizer calibration
#define FSCAL0       0x26    // Frequency Synthesizer calibration
#define RCCTRL1      0x27    // RC oscillator configuration
#define RCCTRL0      0x28    // RC oscillator configuration
#define TEST0        0x2E    // Various test settings

// Strobe Commands
#define SRES         0x30    // Reset Chip
#define SCAL         0x33    // Calibrate Frequency Synthesizer and Turn it off
#define SRX          0x34    // Enable RX. Perform calibration first if coming from IDLE
#define SIDLE        0x36    // Exit RX / TX, turn off Frequency Synthesizer and exit
#define SFRX         0x3A    // Flush RX FIFO

// Status Registers
#define MARCSTATE    0xF5    // Main Radio Control State Machine State

void SpiStrobe(byte strobe);

void SpiWriteReg(byte addr, byte value);

byte SpiReadReg(byte addr);

void initCC1101(void);
