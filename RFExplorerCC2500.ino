#include "cc2500.h"
#include "cc2500_REG.h"
#include "cc2500_VAL.h"

/*
 * Hardware SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCLK/SCK -> 13
 * CSN/SS - > 10
*/
#include <SPI.h>

#define TX_TIMEOUT 10 // Timeouts are added
long previousTXTimeoutMillis = 0;
long previousMillis = 0;
long sendInterval = 400; // in milliseconds

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete


#define CHANNELS 206
uint8_t results[CHANNELS];
uint8_t calibration_FSCAL2 = 0;
uint8_t calibration_FSCAL3 = 0;
uint8_t calibration[CHANNELS];

void setup() {
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(SCK, OUTPUT);
  pinMode(CS, OUTPUT);

  //SPI.setClockDivider(SPI_CLOCK_DIV2);

  SPI.begin();

  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(F("Initializing Wireless.."));
  if (CC2500_INIT()) {
    Serial.println("Error starting receiver");
    //  for (;;);


  }

  dump_radio_info();
  inputString.reserve(200);

  //Current_setup ->#C3-M:<Main_Model>, <Expansion_Model>, <Firmware_Version> <EOL>
  w(("#C2-M:004,255,01.12"));
  //Current_config -> #C2-F:<Start_Freq>, <Freq_Step>, <Amp_Top>,
  //<Amp_Bottom>, <Sweep_Steps>, <ExpModuleActive>, <CurrentMode>, <Min_Freq>, <Max_Freq>, <Max_Span>, <RBW>, <AmpOffset>, <CalculatorMode> <EOL>
  delay(500);
  //w(("#C2-F:2400000,0100000,0000,0120,0004,0,000,2400000,2500000,1000000,10000,0000,0000"));

  //LETOP hoofletter S
  int flat = 20;
  int samples = 80 + flat;


  Serial.print("\r\n");
  //Current_config -> #C2-F:<Start_Freq>, <Freq_Step>, <Amp_Top>,
  //<Amp_Bottom>, <Sweep_Steps>, <ExpModuleActive>, <CurrentMode>, <Min_Freq>, <Max_Freq>, <Max_Span>, <RBW>, <AmpOffset>, <CalculatorMode> <EOL>


  //OFFSET is 071 per datasheet! check http://www.ti.com/lit/an/swra114d/swra114d.pdf
  //RBW is 58khz
  //w(("#C2-F:2399999,0405465,0010,-120,0205,0,000,2399999,2483279,1000000,00406,-071,0000"));
  w(("#C2-F:2399999,0405465,0010,-120,0205,0,000,2399999,2483279,1000000,00058,0000,0000"));

  //  w(("#K0"));
  callibrationDataHandler() ;
  /*
   Serial.println("WAITING FOREVER");
   while(1){

     }
     */

  delay(2000);
  RxModeOff();
}

void dumpSamples(uint8_t *samples, uint8_t count) {
  /*
  * Dump all samples, multiply values by 2
  */
  Serial.write('$');
  Serial.write('S');
  //Serial.print("$S0239");
  //write leng of pending samples
  Serial.write(count);
  for (int x = 0; x < count; x++) {
    //  byte calc = (x * 2) % 20;
    Serial.write(samples[x] * 2); //multiply times 2 as expected by RF-explorer
  }
  Serial.print("\r\n");
}

void testDump() {
  byte cc = 0xFF;

  Serial.write('$');
  Serial.write('S');
  Serial.write(cc);
  for (int x = 0; x < cc; x++) {
    byte calc = (x * 2) % 20;
    Serial.write(calc);
  }
  Serial.print("\r\n");
}

void loop() {
  //  testDump();
  sweep_full(&results[0]);
  //dumpSamples(&results[0], CHANNELS);
  // put your main code here, to run repeatedly:
  serialEvent(); //call the function
  if (stringComplete) {
    //  Serial.println(inputString);
    inputString = "";
    stringComplete = false;
  }
  //  delay(1000);
}

void w(char* s) {
  Serial.write(s);
  Serial.print("\r\n");
}

void callibrationDataHandler() {
  //disable auto-cal
  SPI_Write( MRFI_CC2500_SPI_REG_MCSM0, 0x08);

  for (int x = 0; x < CHANNELS; x++) {
    Serial.print("Calibarating C:");
    Serial.print( x, DEC);

    SPI_Strobe( MRFI_CC2500_SPI_STROBE_SIDLE );  // enter IDLE mode (not needed except for autocal)

    SPI_Write( MRFI_CC2500_SPI_REG_CHANNR, x); // Channel number. Default spacing is ~200KHz/channel

    SPI_Strobe(MRFI_CC2500_SPI_STROBE_SCAL); //run calibration

    //Blijf in idle wachten
    while (SPI_Read(MRFI_CC2500_SPI_REG_MARCSTATE) == STATE_IDLE) {
      // make sure Radio reaches SCAL before while(!IDLE) is called
      Serial.print("C");
    }
    while (SPI_Read(MRFI_CC2500_SPI_REG_MARCSTATE) != STATE_IDLE) {
      // make sure Radio reaches SCAL before while(!IDLE) is called
      Serial.print("R");
    }

    if (x == 0) {
      // we need to store FSCAL2 & FSCAL3 once

      calibration_FSCAL2 = (uint8_t)SPI_Read(MRFI_CC2500_SPI_REG_FSCAL2);
      calibration_FSCAL3 = (uint8_t)SPI_Read(MRFI_CC2500_SPI_REG_FSCAL3);

    }
    calibration[x] = (uint8_t)SPI_Read(MRFI_CC2500_SPI_REG_FSCAL1);
    Serial.print(" FSCAL2: ");
    Serial.print( calibration_FSCAL2, HEX);
    Serial.print(" FSCAL3: ");
    Serial.print( calibration_FSCAL3, HEX);
    Serial.print(" FSCAL1: ");
    Serial.print( calibration[x] , HEX);
    Serial.println("");
    SPI_Strobe( MRFI_CC2500_SPI_STROBE_SIDLE );  // enter IDLE mode (not needed except for autocal)
  }
  Serial.println("calibration done");
}


void sweep_full(uint8_t *data) {

  uint8_t rssi_dec;
  int16_t rssi_dBm;
  uint8_t rssi_offset = 72;
  uint8_t r = 0;

  Serial.write('$');
  Serial.write('S');
  //Serial.print("$S0239");
  //write leng of pending samples
  Serial.write(CHANNELS);

  for (int channel = 0; channel < CHANNELS; channel++)
  {

    //#define DBG
    uint8_t rssi = measure( channel, 200) ;
#ifdef DBG
    //    results[channel] = rssi;
    Serial.print("channel: ");
    Serial.print(channel );//"channel: ");
    Serial.print(" " );
    Serial.println(rssi);
#endif
#ifndef DBG
    Serial.write(rssi * 2);
#endif
  }
  Serial.print("\r\n");

}

uint8_t measure(uint8_t channel, uint8_t maxWaitTime) {
  // RxModeOff();
  SPI_Write(MRFI_CC2500_SPI_REG_FSCAL1, calibration[channel]);
  SPI_Write(MRFI_CC2500_SPI_REG_FSCAL2, calibration_FSCAL2);
  SPI_Write(MRFI_CC2500_SPI_REG_FSCAL3, calibration_FSCAL3);
  // CHANNR = channel;
  SPI_Strobe( MRFI_CC2500_SPI_STROBE_SFRX );

  SPI_Write( MRFI_CC2500_SPI_REG_CHANNR, channel);
  SPI_Strobe(MRFI_CC2500_SPI_STROBE_SRX);

  MRFI_RSSI_VALID_WAIT();

  // delay(12);
  uint8_t reg_value = (uint8_t)SPI_Read(MRFI_CC2500_SPI_REG_RSSI); //GET RSSI

  int rssi_dBm = 0;
  uint8_t rssi_offset = 71;
  //letop moet SIGNED worden ivm calculatie negatieve nummers!!
  int temp = 0;
  uint16_t rssi_dec = (uint16_t)SPI_Read(MRFI_CC2500_SPI_REG_RSSI);
  if (rssi_dec >= 128) {
    rssi_dBm = rssi_dec - 256;
    rssi_dBm = rssi_dBm / 2;
    rssi_dBm = rssi_dBm - rssi_offset;
  }
  else {
    temp = (((int)rssi_dec) / 2) - rssi_offset;

  }

  RxModeOff();
  return (abs(rssi_dBm));

}




void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}




