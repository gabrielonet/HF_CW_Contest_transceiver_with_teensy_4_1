// Emulating Elecraft K3S transceiver CAT codes
// N1MM is expecting a fixed number of characters, if lower or higher, is stalling. 
// so a trailing or leading zeroes is to be implemented to set that fized lenght
// Serial data is not reliable until a CTS, RTS low level is implemented
// If using just blind RX and TX only it is prone to errors, so a higher baud rate is needed ... To be fixed  
int old_millis ;
int x = 5 ;
String freq = "00014175000" ;
String mode = "3" ; // 3 = CW, 2 = USB 

void setup() {
  Serial.begin (115200 );
  Serial5.begin(115200 );
}

void loop() {
   if (Serial5.available() > 0) 
   {
        String data = Serial5.readStringUntil(';') + ";" ;
        int data_lenght = data.length(); 
        
        if (data == "IF;"){
          Serial5.print( "FA" + freq + ";" + "IF" + freq + "     +000000 000" + mode + "000001 ;" ) ;
        }            

        if (data.length() == 14){
          data = data.remove(0,2) ; 
          data = data.remove(11,1);
          Serial.print(data);
          Serial.println("   N1MM says Change Freq");
          freq = data; 
        }
        if (data == "MD3;"){
          data = data.remove(0,2); data = data.remove(1,1); 
          Serial.print(data);
          Serial.println("=N1MM says Change to CW mode"); 
          mode = data ;
        }   
        if (data == "MD2;"){
          data = data.remove(0,2); data = data.remove(1,1); 
          Serial.print(data);          
          Serial.println("=N1MM says Change to USB mode"); 
          mode = data ;
        }   
        if (data == "MD1;"){
          data = data.remove(0,2); data = data.remove(1,1); 
          mode = data ;
          Serial.print(data);
          Serial.println("=N1MM says Change to LSB mode"); 
        }   
     }     


}
