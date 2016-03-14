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


#define CHANNELS 205
uint8_t results[CHANNELS];
uint8_t calibration[CHANNELS][3];

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

  Serial.println(F("RSSI: "));


  sweep_full(&results[0]);
  Serial.println("Sweep Done" );
  for (int i = 0; i < CHANNELS; i++) {
    Serial.print(results[i]);
    Serial.print(",");
  }

  Serial.println(" ");


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



  w(("#C2-F:2399999,0405465,0010,-120,0205,0,000,2399999,2483279,1000000,00406,0000,0000"));

  //  w(("#K0"));
  // callibrationDataHandler() ;
}

void dumpSamples(uint8_t *samples, uint8_t count) {
  /*
  * Dump all samples, multiply values by 2
  */
  Serial.write('$');
  Serial.write('S');
  //Serial.print("$S0239");
  Serial.write(count);
  for (int x = 0; x < count; x++) {
    byte calc = (x * 2) % 20;
    Serial.write(samples[x]);
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
  dumpSamples(&results[0], CHANNELS);
  // put your main code here, to run repeatedly:
  serialEvent(); //call the function
  if (stringComplete) {
    Serial.println(inputString);
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
  for (int x = 0; x < CHANNELS; x++) {
    Serial.print("Calibarating C:");
    Serial.println(x, DEC);
    SPI_Strobe( MRFI_CC2500_SPI_STROBE_SIDLE );  // enter IDLE mode (not needed except for autocal)
    SPI_Write( MRFI_CC2500_SPI_REG_CHANNR, x); // Channel number. Default spacing is ~200KHz/channel
    delay(1);
    SPI_Strobe( MRFI_CC2500_SPI_STROBE_SRX ); // enter Rx mode (not needed except to start autocal)
    delay(20);

    calibration[x][0] = (uint8_t)SPI_Read(MRFI_CC2500_SPI_REG_FSCAL0);
    calibration[x][1] = (uint8_t)SPI_Read(MRFI_CC2500_SPI_REG_FSCAL1);
    calibration[x][2] = (uint8_t)SPI_Read(MRFI_CC2500_SPI_REG_FSCAL2);
    SPI_Strobe( MRFI_CC2500_SPI_STROBE_SIDLE );  // enter IDLE mode (not needed except for autocal)
  }
}


void sweep_full(uint8_t *data) {

  uint8_t rssi_dec;
  int16_t rssi_dBm;
  uint8_t rssi_offset = 72;
  uint8_t r = 0;

  for (int i = 0; i < CHANNELS; i++)
  {
   // Serial.println("-----");
    SPI_Strobe( MRFI_CC2500_SPI_STROBE_SFRX);
  //  Serial.println("srfx");
    //delay(1);
    SPI_Write( MRFI_CC2500_SPI_REG_CHANNR, i); // Channel number. Default spacing is ~200KHz/channel
    //SPI_Write(MRFI_CC2500_SPI_REG_FSCAL0,  calibration[i][0]);
    //SPI_Write(MRFI_CC2500_SPI_REG_FSCAL1,  calibration[i][1]);
    //SPI_Write(MRFI_CC2500_SPI_REG_FSCAL2,  calibration[i][2]);
    //SPI_Strobe( MRFI_CC2500_SPI_STROBE_SCAL);
   // Serial.println("channelnr");
   // delay(10);
    r = SPI_Strobe( MRFI_CC2500_SPI_STROBE_SRX ); // enter Rx mode (not needed except to start autocal)
    //SPI_Write( MRFI_CC2500_SPI_REG_CHANNR, i); // Channel number. Default spacing is ~200KHz/channel
   //Serial.println("src strob");
   //  radio_status(r);
    delay(12);

    //Serial.print(" RSSI: ");
    //rssi_dec = (uint8_t)SPI_Read(MRFI_CC2500_SPI_REG_RSSI);
    rssi_dec = (uint8_t)SPI_Read(MRFI_CC2500_SPI_REG_RSSI);
    if (rssi_dec >= 128)
      rssi_dBm = (int16_t)((int16_t)( rssi_dec - 256) / 2) - rssi_offset;
    else
      rssi_dBm = (rssi_dec / 2) - rssi_offset;

    data[i] = (uint8_t)abs(rssi_dBm) * 2;
   // Serial.print(rssi_dBm, DEC); // Cheap speed hack: write upper 7 bits of RSSI value (throw away LSB). Use LSB to signal start of 256-channel RSSI byte list

    SPI_Strobe( MRFI_CC2500_SPI_STROBE_SIDLE );  // enter IDLE mode (not needed except for autocal)
    _delay_us(100);
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




