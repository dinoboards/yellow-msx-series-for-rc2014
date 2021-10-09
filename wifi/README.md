# Integrating the RC2014 ESP8266 Wifi Module into the MSX build

The Wifi Module can be used by applications within MSX-DOS to connect to the internet, download files, synchronise the MSX's clock

There are a couple of things that need to be undertaken to enable this integration, as the stock build and firmware of the module is designed more for remoting into the system.

1. Update the ESP8266 firmware/software
2. Wire in the SIO's hardware flow control line (RTS) to the ESP8266.

## Updating the ESP8266 firmware

The software written for the ESP8266 module can be found at https://github.com/vipoo/yellow-msx-esp8266-wifi-module

You will need to clone that repo, and ensure you have a serial/FTDI interface between your PC and the module.  See the above repo's readme file for specific instructions.

You can also find alternative instructions for programming on spencer's site at https://rc2014.co.uk/modules/esp8266-wifi-module/esp8266-wifi-module-reprogramming/

And the internet is full of articles on using and programming the ESP8266

## Wire H/W flow control

To ensure clean and reliable data transfer from the ESP8266 to the Zilog SIO chip, you will need to have hardware flow control wired.  Without this, the data sent from the ESP8266 module will overwhelm the SIO/Z80 receive buffers.

To get this wired, you will need to use the prototype are of the module to add some resistors to map the 5V RTS from the SIO/2 to the 3.3V CTS/GPIO13 line.  Flow control is only needed in the one direction - from ESP8266 to SIO/2.  When transmitting from the SIO/2 to the ESP8266, the ESP8266 will easily keep up at the 19200 baud rate.

The following diagram shows the schematic of the change:

<p align="center">
<image src="./schematic.png">
</p>

A header pin is also soldered on the board to allow a jumper wire to connect to the SIO RTS line.

Here are images of my module modified.

<image src="./image1.jpg">
<image src="./image2.jpg">
<image src="./image3.jpg">
