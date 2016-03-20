
#ifndef CC2500_H
#define CC2500_H

//hack to allow uint8_t;
#include <util/delay.h>

//#define LED      13  /* LED connected to digital pin 7 */
#define CS 10       /*10 CS low during transfer*/
#define TEST_VALUE 0x03 /*value for use with pkg-len test */

#define CC2500_WRITE_SINGLE     0x00
#define CC2500_WRITE_BURST      0x40
#define CC2500_READ_SINGLE      0x80
#define CC2500_READ_BURST       0xC0

#define R_STATUS_IDLE 0x00
#define R_STATUS_RX 0x01
#define R_STATUS_TX 0x02
#define R_STATUS_FSTXON 0x03
#define R_STATUS_CALIB 0x04
#define R_STATUS_SETTLING 0x05
#define R_STATUS_RXFIFO_OVERFLOW 0x06
#define R_STATUS_TXFIFO_OVERFLOW 0x07

void RxModeOff(void);
uint8_t getRadioBitStatus(uint8_t c);
void dump_radio_info(void);
void radio_status(uint8_t stat);
uint8_t CC2500_INIT(void);

uint8_t SPI_Strobe(uint8_t reg);
uint8_t SPI_Write(uint8_t reg, uint8_t data);
uint8_t SPI_Read(uint8_t reg);
uint8_t SPI_ReadFifo(uint8_t *data, uint8_t len);


void cc2500_resetChip();

#endif
