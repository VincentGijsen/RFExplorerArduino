/* Packet length mode = Fixed packet length mode. Length configured in PKTLEN register */
/* Address config = No address check */
/* TX power = 0 */
/* RX filter BW = 406.250000 */
/* Whitening = false */
/* Modulation format = MSK */
/* Data rate = 499.878 */
/* Base frequency = 2399.999512 */
/* Sync word qualifier mode = No preamble/sync */
/* Channel spacing = 349.914551 */
/* Device address = 0 */
/* Carrier frequency = 2482.229431 */
/* Modulated = true */
/* Packet length = 255 */
/* Preamble count = 2 */
/* Phase transition time = 0 */
/* Data format = Synchronous serial mode */
/* CRC enable = false */
/* CRC autoflush = false */
/* Channel number = 235 */
/* Manchester enable = false */
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
#define SMARTRF_SETTING_IOCFG2           0x0B //gdo2output pin configuration  
#define SMARTRF_SETTING_IOCFG1           0x2E //gdo1output pin configuration  
#define SMARTRF_SETTING_IOCFG0           0x0C //gdo0output pin configuration  
#define SMARTRF_SETTING_FIFOTHR          0x07 //rx fifo and tx fifo thresholds 
#define SMARTRF_SETTING_SYNC1            0xD3 //sync word, high byte  
#define SMARTRF_SETTING_SYNC0            0x91 //sync word, low byte  
#define SMARTRF_SETTING_PKTLEN           0xFF //packet length  
#define SMARTRF_SETTING_PKTCTRL1         0x00 //packet automation control 
#define SMARTRF_SETTING_PKTCTRL0         0x10 //packet automation control 
#define SMARTRF_SETTING_ADDR             0x00 //device address  
#define SMARTRF_SETTING_CHANNR           0xEB //channel number  
#define SMARTRF_SETTING_FSCTRL1          0x0C //frequency synthesizer control  
#define SMARTRF_SETTING_FSCTRL0          0x00 //frequency synthesizer control  
#define SMARTRF_SETTING_FREQ2            0x5C //frequency control word, high byte  
#define SMARTRF_SETTING_FREQ1            0x4E //frequency control word, middle byte  
#define SMARTRF_SETTING_FREQ0            0xC4 //frequency control word, low byte  
#define SMARTRF_SETTING_MDMCFG4          0x4E //modem configuration  
#define SMARTRF_SETTING_MDMCFG3          0x3B //modem configuration  
#define SMARTRF_SETTING_MDMCFG2          0x70 //modem configuration 
#define SMARTRF_SETTING_MDMCFG1          0x03 //modem configuration 
#define SMARTRF_SETTING_MDMCFG0          0xB9 //modem configuration  
#define SMARTRF_SETTING_DEVIATN          0x00 //modem deviation setting  
#define SMARTRF_SETTING_MCSM2            0x07 //main radio control state machine configuration  
#define SMARTRF_SETTING_MCSM1            0x3C //main radio control state machine configuration 
#define SMARTRF_SETTING_MCSM0            0x18 //main radio control state machine configuration  
#define SMARTRF_SETTING_FOCCFG           0x1D //frequency offset compensation configuration 
#define SMARTRF_SETTING_BSCFG            0x1C //bit synchronization configuration 
#define SMARTRF_SETTING_AGCCTRL2         0xC7 //agc control 
#define SMARTRF_SETTING_AGCCTRL1         0x40 //agc control 
#define SMARTRF_SETTING_AGCCTRL0         0xB0 //agc control 
#define SMARTRF_SETTING_WOREVT1          0x87 //high byte event0 timeout  
#define SMARTRF_SETTING_WOREVT0          0x6B //low byte event0 timeout  
#define SMARTRF_SETTING_WORCTRL          0xF8 //wake on radio control 
#define SMARTRF_SETTING_FREND1           0xB6 //front end rx configuration  
#define SMARTRF_SETTING_FREND0           0x10 //front end tx configuration  
#define SMARTRF_SETTING_FSCAL3           0xEA //frequency synthesizer calibration  
#define SMARTRF_SETTING_FSCAL2           0x0A //frequency synthesizer calibration  
#define SMARTRF_SETTING_FSCAL1           0x00 //frequency synthesizer calibration  
#define SMARTRF_SETTING_FSCAL0           0x19 //frequency synthesizer calibration  
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

