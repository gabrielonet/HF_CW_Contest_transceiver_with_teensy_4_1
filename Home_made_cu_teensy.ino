#include <Encoder.h>
#include <Adafruit_MCP4725.h>
#include <Wire.h>

#include "si5351.h"
#define Si_5351_crystal 25000000 //Si5351 on board crystal frequency
Si5351 si5351;
Adafruit_MCP4725 MCP4725;

long calibration_constant = 8000; //constant will adjust for errors in the 25 MHz crystal master clock
uint64_t SetFreq;
uint32_t TargetFreq, AddFreq;
int const LED = 13;
byte Byte1, Byte2, Byte3, Byte4, Byte5, Byte6, Byte7;

Encoder myEnc(5, 6);
long oldPosition  = -999;
int led = 13;
bool x ;
int dac = 300 ;
float time1 = 0 ;
long freq = 20000000;
void setup() {
    Serial1.begin(9600);
     pinMode(led, OUTPUT);
     si5351.init(SI5351_CRYSTAL_LOAD_10PF, Si_5351_crystal, calibration_constant);
     si5351.set_freq(freq * 100ULL,SI5351_CLK0); 
     si5351.set_freq(  2100000000,SI5351_CLK0); 
     si5351.set_freq(  2300000000,SI5351_CLK2); 
     si5351.drive_strength(SI5351_CLK2, SI5351_DRIVE_2MA);
     si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_2MA);
     si5351.output_enable(SI5351_CLK0, 1); // 1 = enabled, 0 = disabled, nu afecteaza frecventa setata
     si5351.output_enable(SI5351_CLK2, 0); // 1 = enabled, 0 = disabled, nu afecteaza frecventa setata
     MCP4725.begin(0x61);
     MCP4725.setVoltage(4000, false);
}

void loop() {
  long newPosition = myEnc.read() ;
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    long freq = (newPosition *100  +  freq) ;
    Serial1.print("t21.txt=\"" + String(freq)  + "\"" );
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);
    si5351.set_freq(freq* 100ULL, SI5351_CLK0); //100 MHz
    Serial.println(freq* 100ULL);

    
      }

if (millis() - time1 >= 100 ){  
  digitalWrite(led, x);
  time1 = millis();
  x = !x ;
}






}
