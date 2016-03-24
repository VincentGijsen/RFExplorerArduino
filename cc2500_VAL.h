/* Carrier frequency = 2400.010010 */
/* Data rate = 2.39897 */
/* Address config = No address check */
/* Base frequency = 2400.010010 */
/* Deviation = 38.085938 */
/* Data format = Normal mode */
/* Device address = 0 */
/* Packet length = 255 */
/* Channel spacing = 333.251953 */
/* Modulated = true */
/* Preamble count = 4 */
/* CRC enable = false */
/* Sync word qualifier mode = 30/32 sync word bits detected */
/* TX power = 0 */
/* Manchester enable = false */
/* Modulation format = 2-FSK */
/* RX filter BW = 203.125000 */
/* CRC autoflush = false */
/* Packet length mode = Fixed packet length mode. Length configured in PKTLEN register */
/* Channel number = 0 */
/* Whitening = false */
/* PA table */
#define PA_TABLE {0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x00,}
/***************************************************************
 *  SmartRF Studio(tm) Export
 *
 *  Radio register settings specifed with C-code
 *  compatible #define statements.
 *
 *  RF device: CC2500
 *
 ***************************************************************/

#ifndef SMARTRF_CC2500_H
#define SMARTRF_CC2500_H

#define SMARTRF_RADIO_CC2500
#define SMARTRF_SETTING_IOCFG2           0x2E //gdo2output pin configuration  
#define SMARTRF_SETTING_IOCFG1           0x2E //gdo1output pin configuration  
#define SMARTRF_SETTING_IOCFG0           0x2E //gdo0output pin configuration  
#define SMARTRF_SETTING_FIFOTHR          0x07 //rx fifo and tx fifo thresholds 
#define SMARTRF_SETTING_SYNC1            0xD3 //sync word, high byte  
#define SMARTRF_SETTING_SYNC0            0x91 //sync word, low byte  
#define SMARTRF_SETTING_PKTLEN           0xFF //packet length  
#define SMARTRF_SETTING_PKTCTRL1         0x04 //packet automation control 
#define SMARTRF_SETTING_PKTCTRL0         0x00 //packet automation control 
#define SMARTRF_SETTING_ADDR             0x00 //device address  
#define SMARTRF_SETTING_CHANNR           0x00 //channel number  
#define SMARTRF_SETTING_FSCTRL1          0x08 //frequency synthesizer control  
#define SMARTRF_SETTING_FSCTRL0          0x00 //frequency synthesizer control  
#define SMARTRF_SETTING_FREQ2            0x5C //frequency control word, high byte  
#define SMARTRF_SETTING_FREQ1            0x4E //frequency control word, middle byte  
#define SMARTRF_SETTING_FREQ0            0xDE //frequency control word, low byte  
#define SMARTRF_SETTING_MDMCFG4          0x86 //modem configuration  
#define SMARTRF_SETTING_MDMCFG3          0x83 //modem configuration  
#define SMARTRF_SETTING_MDMCFG2          0x03 //modem configuration 
#define SMARTRF_SETTING_MDMCFG1          0x23 //modem configuration 
#define SMARTRF_SETTING_MDMCFG0          0xA4 //modem configuration  
#define SMARTRF_SETTING_DEVIATN          0x44 //modem deviation setting  
#define SMARTRF_SETTING_MCSM2            0x07 //main radio control state machine configuration  
#define SMARTRF_SETTING_MCSM1            0x30 //main radio control state machine configuration 
#define SMARTRF_SETTING_MCSM0            0x18 //main radio control state machine configuration  
#define SMARTRF_SETTING_FOCCFG           0x16 //frequency offset compensation configuration 
#define SMARTRF_SETTING_BSCFG            0x6C //bit synchronization configuration 
#define SMARTRF_SETTING_AGCCTRL2         0x03 //agc control 
#define SMARTRF_SETTING_AGCCTRL1         0x40 //agc control 
#define SMARTRF_SETTING_AGCCTRL0         0x91 //agc control 
#define SMARTRF_SETTING_WOREVT1          0x87 //high byte event0 timeout  
#define SMARTRF_SETTING_WOREVT0          0x6B //low byte event0 timeout  
#define SMARTRF_SETTING_WORCTRL          0xF8 //wake on radio control 
#define SMARTRF_SETTING_FREND1           0x56 //front end rx configuration  
#define SMARTRF_SETTING_FREND0           0x10 //front end tx configuration  
#define SMARTRF_SETTING_FSCAL3           0xA9 //frequency synthesizer calibration  
#define SMARTRF_SETTING_FSCAL2           0x0A //frequency synthesizer calibration  
#define SMARTRF_SETTING_FSCAL1           0x00 //frequency synthesizer calibration  
#define SMARTRF_SETTING_FSCAL0           0x11 //frequency synthesizer calibration  
#define SMARTRF_SETTING_RCCTRL1          0x41 //rc oscillator configuration  
#define SMARTRF_SETTING_RCCTRL0          0x00 //rc oscillator configuration  
#define SMARTRF_SETTING_FSTEST           0x59 //frequency synthesizer calibration control  
#define SMARTRF_SETTING_PTEST            0x7F //production test  
#define SMARTRF_SETTING_AGCTEST          0x3F //agc test  
#define SMARTRF_SETTING_TEST2            0x88 //various test settings  
#define SMARTRF_SETTING_TEST1            0x31 //various test settings  
#define SMARTRF_SETTING_TEST0            0x0B //various test settings  
#define SMARTRF_SETTING_PARTNUM          0x80 //chip id  
#define SMARTRF_SETTING_VERSION          0x03 //chip id  
#define SMARTRF_SETTING_FREQEST          0x00 //frequency offset estimate from demodulator  
#define SMARTRF_SETTING_LQI              0x00 //demodulator estimate for link quality  
#define SMARTRF_SETTING_RSSI             0x00 //received signal strength indication  
#define SMARTRF_SETTING_MARCSTATE        0x00 //main radio control state machine state  
#define SMARTRF_SETTING_WORTIME1         0x00 //high byte of wor time  
#define SMARTRF_SETTING_WORTIME0         0x00 //low byte of wor time  
#define SMARTRF_SETTING_PKTSTATUS        0x00 //current gdoxstatus and packet status  
#define SMARTRF_SETTING_VCO_VC_DAC       0x00 //current setting from pll calibration module  
#define SMARTRF_SETTING_TXBYTES          0x00 //underflow and number of bytes  
#define SMARTRF_SETTING_RXBYTES          0x00 //underflow and number of bytes  
#define SMARTRF_SETTING_RCCTRL1_STATUS   0x00 //last rc oscillator calibration result  
#define SMARTRF_SETTING_RCCTRL0_STATUS   0x00 //last rc oscillator calibration result  

#endif

