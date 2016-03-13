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

void setup() {
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(SCK, OUTPUT);
  pinMode(CS, OUTPUT);

  SPI.setClockDivider(SPI_CLOCK_DIV2);

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

  Serial.println(F("RSSI: "));

  volatile uint8_t rssi_dec;
  volatile int16_t rssi_dBm;
  volatile uint8_t rssi_offset = 72;
  for (int i = 0; i < 200; i++)
  {
    Serial.println("-----");
    SPI_Strobe( MRFI_CC2500_SPI_STROBE_SFRX);
    SPI_Write( MRFI_CC2500_SPI_REG_CHANNR, i); // Channel number. Default spacing is ~200KHz/channel
    radio_status(SPI_Strobe( MRFI_CC2500_SPI_STROBE_SRX ));  // enter Rx mode (not needed except to start autocal)
    delay(10);
  

    Serial.print(" RSSI: ");
    rssi_dec = (uint8_t)SPI_Read(MRFI_CC2500_SPI_REG_RSSI);
    if (rssi_dec >= 128)
      rssi_dBm = (int16_t)((int16_t)( rssi_dec - 256) / 2) - rssi_offset;
    else
      rssi_dBm = (rssi_dec / 2) - rssi_offset;

    Serial.print(rssi_dBm, DEC); // Cheap speed hack: write upper 7 bits of RSSI value (throw away LSB). Use LSB to signal start of 256-channel RSSI byte list

    SPI_Strobe( MRFI_CC2500_SPI_STROBE_SIDLE );  // enter IDLE mode (not needed except for autocal)
  //  Serial.print(" r: ");
  //  radio_status(r);
    delay(10);
    Serial.println("+++++++");
    Serial.println("+++++++");
  }

  Serial.println(" ");


  //Current_setup ->#C3-M:<Main_Model>, <Expansion_Model>, <Firmware_Version> <EOL>
  w(("#C2-M:004,255,01.12"));
  //Current_config -> #C2-F:<Start_Freq>, <Freq_Step>, <Amp_Top>,
  //<Amp_Bottom>, <Sweep_Steps>, <ExpModuleActive>, <CurrentMode>, <Min_Freq>, <Max_Freq>, <Max_Span>, <RBW>, <AmpOffset>, <CalculatorMode> <EOL>
  delay(500);
  w(("#C2-F:2400000,0100000,0000,0120,0004,0,000,2400000,2500000,1000000,10000,0000,0000"));

  //LETOP hoofletter S
  int flat = 20;
  int samples = 80 + flat;

  byte data[7] = {'$' , 'S', 0x04 , 0x011 , 0x11, 0x11, 0x11};
  delay(500);

  for (int x = 0; x < 7; x++) {
    Serial.write(data[x]);
  }

  for (int x = 0; x < flat; x++) {
    //Serial.write(0xF0);
  }

  Serial.print("\r\n");
}



void w(char* s) {
  Serial.write(s);
  Serial.print("\r\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  serialEvent(); //call the function
  if (stringComplete) {
    Serial.println(inputString);
    inputString = "";
    stringComplete = false;
  }
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




