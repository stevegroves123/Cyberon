# Cyberon
## Voice activated music player

Parts:
Arduino Nano Connect PR2040

DFRobot Mini MP3 player + 4Gbyte SD card

NeoPixel Ring (12 Pixels)

Speaker (8 ohm)

This is a system I'm developing for fun, it accepts voice commands then plays music (mp3 files)
I'm using: 
DSpotterSDK_MakerHL.h
NeoPixelConnect.h

I've written my own commands for the MP3 player as the library is too slow and does not offer the commands I'd like to use. 
There are no license files with this, if you want to copy it then you will need to create your own and these are dependant on the serial number of the device you are using.  Visit www.Arduino.cc and look for "Nano Connect 2040" documentation. This contains the instructions you need to create your own voice activated system.  I'm extending the basic system by adding the NeoPixel respone to commands and the MP3 player to carry out the commands.
I'm attempting to fit this into a small box - will provide a picture if I actually get it done.
