BFO si5351 is using Teensy 4.1 I2C bus 2. 
Hence, I had to add and import local si5351.h where I changed wire to wire2.
# import <Wire.h>  remains the same