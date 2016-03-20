#include <util/delay.h>
#include "cc2500.h"
#include "cc2500_REG.h"
#include "cc2500_VAL.h"

static const uint8_t radio_regs_config[][2] =
{
  /* internal radio configuration */
  { MRFI_CC2500_SPI_REG_IOCFG0,    SMARTRF_SETTING_IOCFG0   },
  { MRFI_CC2500_SPI_REG_IOCFG1,    SMARTRF_SETTING_IOCFG1   },
  { MRFI_CC2500_SPI_REG_IOCFG2,    SMARTRF_SETTING_IOCFG2   },
  { MRFI_CC2500_SPI_REG_MCSM0,     SMARTRF_SETTING_MCSM0 /*0x10 | (SMARTRF_SETTING_MCSM0 & (1<<2|1<<3)) */   },
  { MRFI_CC2500_SPI_REG_MCSM1,     SMARTRF_SETTING_MCSM1    },
  { MRFI_CC2500_SPI_REG_PKTLEN,    SMARTRF_SETTING_PKTLEN   },
  { MRFI_CC2500_SPI_REG_PKTCTRL0,  MRFI_CC2500_SPI_REG_PKTCTRL0},
  { MRFI_CC2500_SPI_REG_PKTCTRL1,  MRFI_CC2500_SPI_REG_PKTCTRL1},
  /*  { MRFI_CC2500_SPI_REG_PATABLE,   SMARTRF_SETTING_PATABLE0 }, */
  { MRFI_CC2500_SPI_REG_CHANNR,    SMARTRF_SETTING_CHANNR   },
  /*  { MRFI_CC2500_SPI_REG_FIFOTHR,   0x07 | (SMARTRF_SETTING_FIFOTHR & (1<<4|1<<5|1<<6))  },*/
  /* imported SmartRF radio configuration */
  { MRFI_CC2500_SPI_REG_FSCTRL1,   SMARTRF_SETTING_FSCTRL1  },
  { MRFI_CC2500_SPI_REG_FSCTRL0,   SMARTRF_SETTING_FSCTRL0  },
  { MRFI_CC2500_SPI_REG_FREQ2,     SMARTRF_SETTING_FREQ2    },
  { MRFI_CC2500_SPI_REG_FREQ1,     SMARTRF_SETTING_FREQ1    },
  { MRFI_CC2500_SPI_REG_FREQ0,     SMARTRF_SETTING_FREQ0    },
  { MRFI_CC2500_SPI_REG_MDMCFG4,   SMARTRF_SETTING_MDMCFG4  },
  { MRFI_CC2500_SPI_REG_MDMCFG3,   SMARTRF_SETTING_MDMCFG3  },
  { MRFI_CC2500_SPI_REG_MDMCFG2,   SMARTRF_SETTING_MDMCFG2  },
  { MRFI_CC2500_SPI_REG_MDMCFG1,   SMARTRF_SETTING_MDMCFG1  },
  { MRFI_CC2500_SPI_REG_MDMCFG0,   SMARTRF_SETTING_MDMCFG0  },
  { MRFI_CC2500_SPI_REG_DEVIATN,   SMARTRF_SETTING_DEVIATN  },
  { MRFI_CC2500_SPI_REG_FOCCFG,    SMARTRF_SETTING_FOCCFG   },
  { MRFI_CC2500_SPI_REG_BSCFG,     SMARTRF_SETTING_BSCFG    },
  { MRFI_CC2500_SPI_REG_AGCCTRL2,  SMARTRF_SETTING_AGCCTRL2 },
  { MRFI_CC2500_SPI_REG_AGCCTRL1,  SMARTRF_SETTING_AGCCTRL1 },
  { MRFI_CC2500_SPI_REG_AGCCTRL0,  SMARTRF_SETTING_AGCCTRL0 },
  { MRFI_CC2500_SPI_REG_FREND1,    SMARTRF_SETTING_FREND1   },
  { MRFI_CC2500_SPI_REG_FREND0,    SMARTRF_SETTING_FREND0   },
  { MRFI_CC2500_SPI_REG_FSCAL3,    SMARTRF_SETTING_FSCAL3   },
  { MRFI_CC2500_SPI_REG_FSCAL2,    SMARTRF_SETTING_FSCAL2   },
  { MRFI_CC2500_SPI_REG_FSCAL1,    SMARTRF_SETTING_FSCAL1   },
  { MRFI_CC2500_SPI_REG_FSCAL0,    SMARTRF_SETTING_FSCAL0   },
  { MRFI_CC2500_SPI_REG_TEST2,     SMARTRF_SETTING_TEST2    },
  { MRFI_CC2500_SPI_REG_TEST1,     SMARTRF_SETTING_TEST1    },
  { MRFI_CC2500_SPI_REG_TEST0,     SMARTRF_SETTING_TEST0    },

  /*JMK:*/
  { MRFI_CC2500_SPI_REG_IOCFG0, SMARTRF_SETTING_IOCFG0 },
  { MRFI_CC2500_SPI_REG_FIFOTHR, SMARTRF_SETTING_FIFOTHR /*RX>8*/},
  { MRFI_CC2500_SPI_REG_PKTCTRL1, MRFI_CC2500_SPI_REG_PKTCTRL1/*1<<2*/ /*Append status, no addr check*/ },
  { /*MRFI_CC2500_SPI_REG_MCSM0, 1<<4|1<<2*/ },
  //{ MRFI_CC2500_SPI_REG_PKTLEN, 16 },
  { MRFI_CC2500_SPI_REG_PATABLE, 0xfe },
};

\



uint8_t getRadioBitStatus(uint8_t c) {
  //Serial.print("raw: ");
  // Serial.println(c, BIN);
  uint8_t tVal = c << 1;
  tVal = tVal >> 5;
  //Serial.print("bit: ");
  //Serial.println(tVal, DEC);

  return tVal;
}

void radio_status(uint8_t stat) {

  Serial.print(F("Radio Status: "));
  Serial.print(stat, BIN);
  Serial.print(" ");


  if (stat & 0x80) {
    Serial.print(F("Radio ERROR"));
  }
  uint8_t tVal = getRadioBitStatus(stat);

  switch (tVal) {
    case 0: Serial.print(F("Radio IDLE")); break;
    case 1: Serial.print(F("Radio RX")); break;
    case 2: Serial.print(F("Radio TX")); break;
    case 3: Serial.print(F("Radio FSTXON")); break;
    case 4: Serial.print(F("Radio Calibrate")); break;
    case 5: Serial.print(F("Radio SETTLING")); break;
    case 6: Serial.print(F("Radio RXFIFO_overflow")); break;
    case 7: Serial.print(F("Radio TXFIFO_overflow")); break;
  }
  Serial.println("");
}

void dump_radio_info(void)
{
  Serial.print("Modulation format is ");
  switch ((SPI_Read(MRFI_CC2500_SPI_REG_MDMCFG2) & (1 << 4 | 1 << 5 | 1 << 6)) >> 4)
  {
    case 0: Serial.print("2-FSK");  break;
    case 1: Serial.print("GFSK"); break;
    case 3: Serial.print("OOK");  break;
    case 7: Serial.print("MSK");  break;
  }
  Serial.println("");
  Serial.print("Digital DC blocking is ");
  Serial.print( (SPI_Read(MRFI_CC2500_SPI_REG_MDMCFG2) & (1 << 7)) ? "disabled\n" : "enabled\n" );
  Serial.print("Manchester is ");
  Serial.print( (SPI_Read(MRFI_CC2500_SPI_REG_MDMCFG2) & (1 << 3)) ? "enabled\n" : "disabled\n" );
  Serial.print("Data whitening is ");
  Serial.print( (SPI_Read(MRFI_CC2500_SPI_REG_PKTCTRL0) & (1 << 6)) ? "enabled\n" : "disabled\n" );
  Serial.print("FEC is ");
  Serial.print( (SPI_Read(MRFI_CC2500_SPI_REG_MDMCFG1) & (1 << 7)) ? "enabled\n" : "disabled\n" );

  for (int i = 0; i  < /*sizeof(radio_regs_config) / sizeof(radio_regs_config[0]) */ 10; ++i )
  {
    Serial.print("REG: id:");
    Serial.print(i, DEC);
    Serial.print(" regVal: ");
    Serial.print(radio_regs_config[i][0], HEX);
    Serial.print(" radio: ");
    Serial.println(SPI_Read(radio_regs_config[i][0] ), HEX);
  }
}


uint8_t CC2500_INIT(void) {
  cc2500_resetChip();
  cc2500_resetChip();
  cc2500_resetChip();
  Serial.println(F("PartNR: "));

  uint8_t i = SPI_Read(MRFI_CC2500_SPI_REG_PARTNUM );
  Serial.println(i, HEX);
  delay(10);
  Serial.print("Version ");
  i = SPI_Read(MRFI_CC2500_SPI_REG_VERSION );
  Serial.println(i, HEX);

  //pomp alle registers naar CC2500
  for (int i = 0; i < sizeof(radio_regs_config) / sizeof(radio_regs_config[0]); ++i )
  {
    SPI_Write( radio_regs_config[i][0], radio_regs_config[i][1] );
  }

  Serial.println(F("Done Reg Init"));
  SPI_Strobe(MRFI_CC2500_SPI_STROBE_SIDLE);
  Serial.println(F("Stroble Idle"));
  //  WriteReg(MRFI_CC2500_SPI_REG_CHANNR, 1);
  SPI_Strobe(MRFI_CC2500_SPI_STROBE_SRX);
  Serial.println(F("Stroble SRX"));

  uint8_t r = SPI_Strobe(MRFI_CC2500_SPI_STROBE_SRX);
  delay(1);  // Time needed to activate rx
  radio_status(r);

  //check if we are in RX mode
  delay(10);
  // r = SPI_Strobe(MRFI_CC2500_SPI_STROBE_SNOP);
  // radio_status(r);
  /*
  Serial.println("Testing function" );
  //SPI_Write( MRFI_CC2500_SPI_REG_PKTLEN, TEST_VALUE );
  Serial.println("W" );
  delay(100);
  Serial.println("R");
  r = SPI_Read (MRFI_CC2500_SPI_REG_PKTLEN) ;
  Serial.println("COMPARE");
  if (r != TEST_VALUE)
  {
    Serial.print("Invalid: ");
    Serial.print(TEST_VALUE, HEX);
    Serial.print(" " );
    Serial.println(r, HEX);

    // return 1;
  } else {
    Serial.println("PKQlen_ok");
  }
  */
}


#define MRFI_RSSI_VALID_DELAY_US    1000

void MRFI_STROBE_IDLE_AND_WAIT(void) {
  SPI_Strobe( MRFI_CC2500_SPI_STROBE_SIDLE );
  while (SPI_Strobe(MRFI_CC2500_SPI_STROBE_SNOP) & 0xF0) {}
}


void MRFI_RSSI_VALID_WAIT(void) {
  {
    int16_t dTime = MRFI_RSSI_VALID_DELAY_US;
    do
    {
      if (SPI_Read(MRFI_CC2500_SPI_REG_PKTSTATUS) & ((PKTSTATUS_CCA | PKTSTATUS_CS)))
      {
        break;
      }
      _delay_us(64); /* sleep */
      dTime -= 64;
    } while (dTime > 0);
  }
}


#define MRFI_RSSI_OFFSET 71

void RxModeOff(void) {
  //idle
  MRFI_STROBE_IDLE_AND_WAIT();
  //flush fifo
  SPI_Strobe( MRFI_CC2500_SPI_STROBE_SFRX );
}

int8_t Mrfi_CalculateRssi(uint8_t rawValue)
{
  int16_t rssi;

  /* The raw value is in 2's complement and in half db steps. Convert it to
   * decimal taking into account the offset value.
   */
  if (rawValue >= 128)
  {
    rssi = (int16_t)(rawValue - 256) / 2 - MRFI_RSSI_OFFSET;
  }
  else
  {
    rssi = (rawValue / 2) - MRFI_RSSI_OFFSET;
  }

  /* Restrict this value to least value can be held in an 8 bit signed int */
  if (rssi < -128)
  {
    rssi = -128;
  }

  return rssi;
}

uint8_t SPI_ReadFifo(uint8_t *data, uint8_t len)
{
  uint8_t reg_val;
  digitalWrite(CS, 0);                // CSN low, initialize SPI communication...
  SPI.transfer(MRFI_CC2500_SPI_REG_RXFIFO | CC2500_READ_BURST);    // Select register to read from..
  reg_val = SPI.transfer(0);                 // ..then read register value
  while (len)
  {
    *data = SPI.transfer(0xFF);
    data++;
    len--;
  }
  digitalWrite(CS, 1);                // CSN high, terminate SPI communication
  return (reg_val);                    // return register value
}

uint8_t SPI_Read(uint8_t reg)
{
  uint8_t reg_val;
  digitalWrite(CS, LOW);                // CSN low, initialize SPI communication...
  while (digitalRead(MISO) == HIGH) {};
  SPI.transfer(reg | CC2500_READ_SINGLE | CC2500_READ_BURST );   // Select register to read from..
  reg_val = SPI.transfer(0);                 // ..then read register value
  digitalWrite(CS, HIGH);                // CSN high, terminate SPI communication
  return (reg_val);                    // return register value
}

uint8_t SPI_Strobe(uint8_t reg)
{
  uint8_t reg_val;
  digitalWrite(CS, LOW);                // CSN low, initialize SPI communication...
  while (digitalRead(MISO) == HIGH) {};
  SPI.transfer(reg | CC2500_READ_SINGLE);                         // Select register to read from..
  _delay_us(10);
  reg_val = SPI.transfer(0);                 // ..then read register value
  digitalWrite(CS, HIGH);                // CSN high, terminate SPI communication
  return (reg_val);                    // return register value
}

uint8_t SPI_Write(uint8_t reg, uint8_t data)
{
  uint8_t reg_val;
  digitalWrite(CS, LOW);                // CSN low, initialize SPI communication...
  while (digitalRead(MISO) == HIGH) {};
  SPI.transfer(reg);                         // Select register to read from..
  SPI.transfer(data );                         // Select register to read from..
  reg_val = SPI.transfer(0);                 // ..then read register value
  digitalWrite(CS, HIGH);                // CSN high, terminate SPI communication
  return (reg_val);                    // return register value
}

void cc2500_resetChip(void)
{
  // Toggle chip select signal
  digitalWrite(CS, HIGH);
  _delay_us(30);
  digitalWrite(CS, LOW);
  _delay_us(30);
  digitalWrite(CS, HIGH);
  _delay_us(45);

  //send chip-reset cmd
  SPI_Strobe(MRFI_CC2500_SPI_STROBE_SRES);
  _delay_ms(100);
}
