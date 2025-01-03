// Warning ! MCP4725 DAC has only 2 addresses available, unfortunately one of it is the same as Si5351
// Work in progress to use distinct wire, wire1, wire2 i2c bus to avoid this issue in order to add a secondary DAC !

/* SotaBeams DSP encoder increments : 
        Centre frequency / Hz:
        200-2000: step size 25
        2000-3500: step size 50
        Bandwidth / Hz:
        200-400: step size 20
        400-700: step size 50
        700-3500: step size 100
*/

#include <Encoder.h>
#include <Wire.h>
#include "Adafruit_MCP4725-1.h"
#include "Adafruit_MCP4725-2.h"

#include "si5351.h"
#define Si_5351_crystal 25000000 //Si5351 on board crystal frequency

Adafruit_MCP4725_1 dac1; // MCP4725 on i2c bus1 to VFO board, TX level
Adafruit_MCP4725_2 dac2; // MCP4725 on i2c bus0 to VFO board, RX level
Adafruit_MCP4725_2 dac3; // MCP4725 on i2c bus0 to AGC board

Si5351 si5351;
Encoder myEnc(5, 6);
Encoder DspEnc(7, 8);
int const LED = 13;
long calibration_constant = 8000; //constant will adjust for errors in the 25 MHz crystal master clock
long oldPosition  = -999;
long old_dsp_freq = 0 ;
long old_dsp_bw   = 0 ;
int led = 13;
bool led_bol;
int dac = 300 ;
float time1 = 0 ;
long vfo_rx  = 16001000; 
long bfo     =  9001000;
long freq_rx = vfo_rx - bfo ;
int  beat_tone = 600 ;
int kkt1 ;
int kkt2 ; 
int old_millis = millis();
int dsp_mode = 0 ; // 0 is BW mode, 1 is Center Mode

void setup() {
     Serial1.begin(9600);
     pinMode(led, OUTPUT);
     pinMode(9, OUTPUT); pinMode(10, OUTPUT); pinMode(11, OUTPUT); digitalWrite(9, 0); digitalWrite(10, 0); digitalWrite(11, 0) ;
     dac1.begin(0x60);
     dac2.begin(0x60);
     dac1.setVoltage(4095, false);
     dac2.setVoltage(0, false);
     Serial.println(freq_rx * 100ULL);
          
     // SET RX Frequency
     si5351.init(SI5351_CRYSTAL_LOAD_10PF, Si_5351_crystal, calibration_constant);
     si5351.set_freq( freq_rx * 100ULL, SI5351_CLK0); 
     si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_2MA);
     si5351.output_enable(SI5351_CLK0, 1); // 1 = enabled, 0 = disabled -> toggling 0/1 does not alter set frequency 


      //  SET TX Frecuency    
      si5351.set_freq(  freq_rx,SI5351_CLK2); 
      si5351.drive_strength(SI5351_CLK2, SI5351_DRIVE_2MA);
      si5351.output_enable(SI5351_CLK2, 0); // 1 = enabled, 0 = disabled -> toggling 0/1 does not alter set frequency 

      //Dsp_mode should be set to BW by now, Set DSP BW to minimum.
      Serial.println(millis());
      for (int i = 0; i < 100 ; i++){
            // Emulate Gray code output sequence for one detent
            digitalWrite(9, 0); digitalWrite(10, 1); delay (1) ;
            digitalWrite(9, 1); digitalWrite(10, 1); delay (1) ;
            digitalWrite(9, 1); digitalWrite(10, 0); delay (1) ;
            digitalWrite(9, 0); digitalWrite(10, 0); delay (1) ;
        }
      // Raise DSP Bandwidth to 600 Hz
      for (int i = 0; i <= 24 ; i++){
            // Emulate Gray code output sequence for one detent
            digitalWrite(9, 0); digitalWrite(10, 1); delay (1) ;
            digitalWrite(9, 1); digitalWrite(10, 1); delay (1) ;
            digitalWrite(9, 1); digitalWrite(10, 0); delay (1) ;
            digitalWrite(9, 0); digitalWrite(10, 0); delay (1) ;
      }
      
      // Set DSP mode to "Center" and reduce it to minimum then raise it to 600 Hz
      digitalWrite(11, 1); digitalWrite(11, 0);
      for (int i = 0; i < 100 ; i++){
            // Emulate Gray code output sequence for one detent
            digitalWrite(9, 0); digitalWrite(10, 1); delay (1) ;
            digitalWrite(9, 1); digitalWrite(10, 1); delay (1) ;
            digitalWrite(9, 1); digitalWrite(10, 0); delay (1) ;
            digitalWrite(9, 0); digitalWrite(10, 0); delay (1) ;
        }  
      // Raise DSP Center to 600 Hz
      for (int i = 0; i <= 24 ; i++){
            // Emulate Gray code output sequence for one detent
            digitalWrite(9, 0); digitalWrite(10, 1); delay (1) ;
            digitalWrite(9, 1); digitalWrite(10, 1); delay (1) ;
            digitalWrite(9, 1); digitalWrite(10, 0); delay (1) ;
            digitalWrite(9, 0); digitalWrite(10, 0); delay (1) ;
      }
      // Set DSP mode back to BW mode
      digitalWrite(11, 1); digitalWrite(11, 0);  
      Serial.println(millis());
      
}

void loop() {
  long newPosition = myEnc.read()/1 ;
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial1.print("t21.txt=\"" + String(freq_rx)  + "\"" );
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);
    si5351.set_freq((vfo_rx + newPosition + beat_tone) * 100ULL, SI5351_CLK0); // Set Clock 0 to generate RX output, for 7 Mhz that would be 16 Mhz plus 600 Hz audio tone.
    si5351.set_freq(((vfo_rx + newPosition) - bfo) * 100ULL, SI5351_CLK2);                      // Set Clock 2 to generate TX output which is freq_rx
    //Serial.println( String((freq_rx + newPosition + beat_tone)) + " Khz") ;
    Serial.println( (vfo_rx + newPosition - bfo ));
    }



  long  new_dsp_freq = DspEnc.read()  ;
  if (new_dsp_freq != old_dsp_freq && old_millis < millis() -50) {
    old_dsp_freq = new_dsp_freq ;
    Serial.println(new_dsp_freq / 4);
    old_millis = millis() ;

    digitalWrite(9, 0);
    
    }    
    
    // Led blink only, using Delta time not delay function !
    if (millis() - time1 >= 100 ){  
        digitalWrite(led, led_bol);
        time1 = millis();
        led_bol = !led_bol ;
     }
               
    
}
