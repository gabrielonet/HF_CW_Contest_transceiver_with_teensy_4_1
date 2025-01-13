https://yo8rxp.com/home-made-transceiver-rxp2/
<li>This would be my second home made HF transceiver. Contest transceiver 6 bands, cw only, Kenwood codes emulation for PC interfacing.
<li>I/Q quadrature + 9 mhz crystal filter I.F. stage
<li>H-mode RF mixer
<li>Si5351 VFO but heavy filtered for each band, TX and RX (more than 60 dB second harmonic rejection)
<li>Teensy controller for logic
<li>Teensy + audio shield as Hilbert DSP for LSB / USB rejection
<li>// N1MM is expecting a fixed number of characters, if lower or higher, it is stalling. 
<li>// so a trailing or leading zeroes is to be implemented to set that fized lenght
<li>// Serial data is not reliable until a CTS, RTS signaling low level is implemented
<li>// If using just blind RX and TX only, it is prone to errors, so a higger baud rate is needed ... To be fixed  
