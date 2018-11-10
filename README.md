# RFExplorerArduino
Arduino + CC2500 to emulate Rfexplorer hardware, note that this is NOT a replacement for the real deal

This code emulates the RFExplorer, only 2400 - 2483mhz.

I've used a arduino 8mhz, 3.3 volt (please be aware that the CC2500 is NOT 5V tolerant!).


This repo is meant to be pared with a forked version of the excelent PC GUI: RFExplorer (check out https://github.com/VincentGijsen/rfexplorer). I've made very small changes, so all credits for the GUI to the original authors, I mearly changed minor stuff regarding the recognition of other serial interfaces, and baudrates, supported by the Arduino.

For some background info, check http://www.vincentgijsen.nl
