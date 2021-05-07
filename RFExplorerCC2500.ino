#include "cc2500.h"
#include "cc2500_REG.h"
#include "cc2500_VAL.h"
#include <math.h> 
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

char inputString[255];         // a string to hold incoming data
int i = 0;
boolean stringComplete = false;  // whether the string is complete
#define MAX_SAMPLING 1

uint8_t channels = 250;
#define RSSI_OFFSET 72

uint8_t results[250];
uint8_t calibration_FSCAL2 = 0;
uint8_t calibration_FSCAL3 = 0;
uint8_t calibration[250];

#define MINFREQUENCY 2400010
#define MAXFREQUENCY 2483323
long StartFrequency = 2400010;
long EndFrequency = 2483323;

boolean speedMode = true;

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
  //inputString.reserve(200);

  //Current_setup ->#C3-M:<Main_Model>, <Expansion_Model>, <Firmware_Version> <EOL>
  //werkend!
  w(("#C2-M:004,255,01.15"));



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

  long freqRegister =  6145096;
  float divider = 0.000396728515625;

  Serial.println(prependZeros(250, 4));

  //EndFrequency = 2463324;
  //OFFSET is 071 per datasheet! check http://www.ti.com/lit/an/swra114d/swra114d.pdf
  //RBW is 58khz
  //w(("#C2-F:2399999,0405465,0010,-120,0205,0,000,2399999,2483279,1000000,00406,-071,0000"));
  //WORKING line!
  //w(("#C2-F:2400010,0333252,0010,-120,0250,0,000,2400010,2483323,1000000,00203,0000,0000"));
  
  //test line for wifi:
  //w(("#C2-F:2399999,6000000,0010,-120,0013,0,002,2399999,2483279,1000000,00203,0000,0000"));
  //  w(("#K0"));
  callibrationDataHandler();
/*
  Serial.println("WAITING FOREVER");
  while (1) {
    testDump(13);
    delay(2000);
  }
*/
}

long calculateFrequencyStep(long startFreq, long endFreq, uint8_t steps){
  return ((endFreq - startFreq) * 1000 / steps);
}

long calculateMaxFrequenzy(long baseFrequency, long freqStep, uint8_t steps) {
    return baseFrequency + ((float)(freqStep *  steps ) / 1000);
}

long setChannelSpacing(long spacing) {
  uint8_t chanspc_m = 0;
  uint8_t chanspc_e = 0;

  if(spacing > 25390){
    if(spacing <= 50682) {
      chanspc_m = int((spacing - 25390) / 99.182);
    }
    else if(spacing <= 101364) {
      chanspc_m = int((spacing - 50781) / 198.364);
      chanspc_e = 1;
    }
    else if(spacing <= 202728) {
      chanspc_m = int((spacing - 101562) / 396.728);
      chanspc_e = 2;
    }
    else {
      chanspc_m = int((spacing - 203125) / 793.457);
      chanspc_e = 3;
    }
  }
  Serial.println(spacing);
  Serial.println(chanspc_m);
  Serial.println(chanspc_e);
  SPI_Write(MRFI_CC2500_SPI_REG_MDMCFG0, chanspc_m);
  SPI_Write(MRFI_CC2500_SPI_REG_MDMCFG1,SPI_Read(MRFI_CC2500_SPI_REG_MDMCFG1) & (0b11111100 | chanspc_e));

  return 0.00009918212890625 * (256 + chanspc_m) * pow(2, chanspc_e) * 1000 * 1000;
}

long caculcateChannelSpacing(uint8_t chansp, uint8_t chanspExp) {
  return 0.00009918212890625 * (256 + chansp) * pow(2, chanspExp) * 1000;
}

void setBaseFrequency(long frequency) {
  unsigned char regValues[4];
  longToByte(regValues, (long)(frequency / 1000 / 0.000396728515625));

  SPI_Write(MRFI_CC2500_SPI_REG_FREQ0, regValues[0]);
  SPI_Write(MRFI_CC2500_SPI_REG_FREQ1, regValues[1]);
  SPI_Write(MRFI_CC2500_SPI_REG_FREQ2, regValues[2]);
}

void printHex(uint8_t num) {
  char hexCar[2];

  sprintf(hexCar, "%02X", num);
  Serial.print(hexCar);
}

String prependZeros(long input, uint8_t len){
  char mystr[7];
  String output = "";
  sprintf(mystr,"%lu",input);
  
  int l = strlen(mystr);
  
  for(i=l;i < len; i++){
      output += "0";
  }
  
  output += mystr;
  return output;
}

boolean StartWith(char*pLine, const char* pLineCompare)
{
    return (0==strncmp(pLine,pLineCompare,strlen(pLineCompare)));
}

void longToByte(byte* arr, long value)
{
      long n = value;

      arr[0] = n & 0x00FF;
      arr[1] = (n >> 8) & 0x00FF;
      arr[2] = (n >> 16) & 0x00FF;
      arr[3] = n >> 24;
}

float byteToLong(unsigned char* arr)
{
      int i = arr[0] | (arr[1] << 8) | (arr[2] << 16) | (arr[3] << 24);
      return *(long*) &i;
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

void testDump(uint8_t count) {
  //haalt +- 90/s loop
  byte cc = 0xFF;

  Serial.write('$');
  Serial.write('S');
  Serial.write(count);
  for (int x = 0; x < count; x++) {
    byte calc = random(1,180);//) % 30;
    Serial.write(calc);
  }
  Serial.print("\r\n");
}

void loop() {
  //testDump();
  sweep_full();
  //dumpSamples(&results[0], CHANNELS);
  // put your main code here, to run repeatedly:
  serialEvent(); //call the function
  
  //  delay(1000);
}

void w(char* s) {
  Serial.write(s);
  Serial.print("\r\n");
}

void callibrationDataHandler() {  
  //disable auto-cal
  SPI_Write( MRFI_CC2500_SPI_REG_MCSM0, 0x08);

  for (int x = 0; x < channels; x++) {
    Serial.flush();
    Serial.print("C:");
    Serial.print( x, DEC);

    SPI_Strobe( MRFI_CC2500_SPI_STROBE_SIDLE );  // enter IDLE mode (not needed except for autocal)

    SPI_Write( MRFI_CC2500_SPI_REG_CHANNR, x); // Channel number. Default spacing is ~200KHz/channel

    SPI_Strobe(MRFI_CC2500_SPI_STROBE_SCAL); //run calibration

    //Blijf in idle wachten
    while (SPI_Read(MRFI_CC2500_SPI_REG_MARCSTATE) == STATE_IDLE) {
      // make sure Radio reaches SCAL before while(!IDLE) is called
      Serial.print("C");
    }
    delayMicroseconds(800);
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
  Serial.flush();
  delay(200);
  RxModeOff();

  SPI_Strobe(MRFI_CC2500_SPI_STROBE_SRX);
}


void sweep_full() {

  uint8_t rssi_dec;
  int16_t rssi_dBm;

  uint8_t r = 0;

  Serial.write('$');
  Serial.write('S');
  //Serial.print("$S0239");
  //write leng of pending samples

  Serial.write(channels);
  uint8_t lastVal = 0;
  for (int channel = 0; channel < channels; channel++)
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
    // software expects 1/2 resolution
    if (rssi != 0 ) {
      Serial.write(rssi * 2);
      lastVal = rssi;
    }
    else {
      Serial.write(lastVal);
    }

#endif
  }
  Serial.print("\r\n");
}

uint8_t measure(uint8_t channel, uint8_t maxWaitTime) {
  int rssi_dBm = 0;
  int RSSI_max = -120;

  
  SPI_Write(MRFI_CC2500_SPI_REG_CHANNR, channel);
  SPI_Write(MRFI_CC2500_SPI_REG_FSCAL1, calibration[channel]);
  SPI_Write(MRFI_CC2500_SPI_REG_FSCAL2, calibration_FSCAL2);
  SPI_Write(MRFI_CC2500_SPI_REG_FSCAL3, calibration_FSCAL3);

  if(!speedMode){
    SPI_Strobe(MRFI_CC2500_SPI_STROBE_SRX);
    MRFI_RSSI_VALID_WAIT();
  }
  else {
    delayMicroseconds(120);
  }

  //letop moet SIGNED worden ivm calculatie negatieve nummers!!
  uint16_t rssi_dec = (uint16_t)SPI_Read(MRFI_CC2500_SPI_REG_RSSI);
  if (rssi_dec >= 128) {
    rssi_dBm = rssi_dec - 256;
    rssi_dBm = rssi_dBm / 2;
    rssi_dBm = rssi_dBm - RSSI_OFFSET;
  }
  else {
    rssi_dBm = (((int)rssi_dec) / 2) - RSSI_OFFSET;
  }

  if (rssi_dBm > RSSI_max) RSSI_max = rssi_dBm; // keep maximum   

  if(!speedMode){
    RxModeOff();
  }
  
  return (abs(RSSI_max));
}

void serialEvent() {
  
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString[i] = inChar;
    i++;
    
    if (inputString[0] == '#') {
      uint8_t len = inputString[1] + 0;

      if( i > 1 && i == len) {
          Serial.println(inputString);
      
          char* line = NULL;
          line = &inputString[0];
          line += 2;

          if (StartWith(line,"C2-F:")) //Received Configuration of Analyzer
          {
              line+=5;
              line[7]=0x00;
              StartFrequency=atol(line);
              line+=8;
              line[7]=0x00;
              EndFrequency=atol(line);

              if(StartFrequency < MINFREQUENCY)
                StartFrequency = MINFREQUENCY;

              if(EndFrequency > MAXFREQUENCY)
                EndFrequency = MAXFREQUENCY;

              setBaseFrequency(StartFrequency);
              long channelStep = calculateFrequencyStep(StartFrequency,EndFrequency,channels);
              channelStep = setChannelSpacing(channelStep);
              
              callibrationDataHandler();
              
              Serial.print((String)"#C2-F:"+StartFrequency+","+prependZeros(channelStep, 7) +",0010,-120,"+prependZeros(channels,4)+",0,000,2400010,2483323,0008331,00203,0000,0000\r\n");
          }
          else if(StartWith(line,"C0")) {
            Serial.print((String)"#C2-F:"+StartFrequency+","+prependZeros(calculateFrequencyStep(StartFrequency,EndFrequency,channels), 7) +",0010,-120,"+prependZeros(channels,4)+",0,000,2400010,2483323,0008331,00203,0000,0000\r\n");
          }
          else if(StartWith(line,"CJ")) {
              line+=3;
              uint8_t c = atoi(line);

              if(c < 1) {
                c = 1;
              }

              if(c > 250) {
                c = 250;
              }

              channels = c;

              long channelStep = calculateFrequencyStep(StartFrequency,EndFrequency,channels);
              channelStep = setChannelSpacing(channelStep);
              EndFrequency = calculateMaxFrequenzy(StartFrequency, channelStep, channels);
              
              callibrationDataHandler();
              Serial.print((String)"#C2-F:"+StartFrequency+","+prependZeros(channelStep, 7) +",0010,-120,"+prependZeros(channels,4)+",0,000,2400010,2483323,0008331,00203,0000,0000\r\n");
          }
          /*else if(StartWith(line,"SP")) {
            line+=3;
            if(atoi(line)) {
              speedMode = true;

              SPI_Strobe(MRFI_CC2500_SPI_STROBE_SRX);
              MRFI_RSSI_VALID_WAIT();
            }
            else {
              speedMode = false;
              RxModeOff();
            }
          }*/
          line = NULL;
          memset(inputString, 0, sizeof(inputString));
          i = 0;
      }
      else if(i > 1){
        //stringComplete = false;
        //memset(inputString, 0, sizeof(inputString));
        //i = 0;
      }
    }
    else {
      stringComplete = false;
      memset(inputString, 0, sizeof(inputString));
      i = 0;
    }
    
  }
}
