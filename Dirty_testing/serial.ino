// N1MM is expecting a fixed number of characters, if lower or higher, is stalling. 
// so a trailing or leading zeroes is to be implemented to set that fized lenght
// Serial data is not reliable until a CTS, RTS low level is implemented
// If using just blind RX and TX only it is prone to errors, so a higher baud rate is needed ... To be fixed  

int x = 5 ;
void setup() {
  Serial.begin (115200);
  Serial5.begin(115200);
}

void loop() {
   if (Serial5.available() > 0) {
        // Read incoming data
        String data = Serial5.readStringUntil(';'); 
        //Serial.println(data); 
        Serial5.print("FA0002100" +  String (x) + "000" + ";" ) ;
    }
}
