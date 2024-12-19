// Warning ! MCP4725 DAC has only 2 addresses available, unfortunately one of it is the same as Si5351
// Work in progress to use distinct wire, wire1, wire2 i2c bus to avoid this issue !
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
long vfo_rx  = 16001000; 
long bfo     =  9001000;
long freq_rx = vfo_rx - bfo ;

void setup() {
    Serial1.begin(9600);
     pinMode(led, OUTPUT);
     MCP4725.begin(0x61);
     MCP4725.setVoltage(1024, false);
     Serial.println(freq_rx * 100ULL);
          
     // SET RX Frequency
     si5351.init(SI5351_CRYSTAL_LOAD_10PF, Si_5351_crystal, calibration_constant);
     si5351.set_freq( freq_rx * 100ULL, SI5351_CLK0); 
     si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_2MA);
     si5351.output_enable(SI5351_CLK0, 1); // 1 = enabled, 0 = disabled -> toggling 0/1 does not alter set frequency 


      // SET TX Frecuency    
      //si5351.set_freq(  2100000000,SI5351_CLK0); 
      //si5351.set_freq(  2300000000,SI5351_CLK2); 
      //si5351.set_freq(1400000000ULL, SI5351_CLK0);
      //si5351.drive_strength(SI5351_CLK2, SI5351_DRIVE_2MA);
      //si5351.output_enable(SI5351_CLK2, 0); // 1 = enabled, 0 = disabled -> toggling 0/1 does not alter set frequency 
}

void loop() {
  long newPosition = myEnc.read() ;
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    //freq_rx = (newPosition +  freq_rx) ;
    Serial1.print("t21.txt=\"" + String(freq_rx)  + "\"" );
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);
    si5351.set_freq((vfo_rx + newPosition) * 100ULL, SI5351_CLK0);
    Serial.println( (freq_rx + newPosition) * 100ULL);
    }
    
    // Led blink only, using Delta time not delay function !
    if (millis() - time1 >= 100 ){  
        digitalWrite(led, x);
        time1 = millis();
        x = !x ;
    }
}
