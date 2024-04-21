# Basic Hardware BOM

This is a somewhat flexible list of what you'll need in terms of hardware to build this mod.

* Brocade/Ruckus/Commscope switch which uses FAN10-MOD (ICX6610, ICX7450, etc.)
* A piece of acrylic to replace the top panel; an acrylic or steel part via PCBWay is in the works
* Teensy 4.1 from PJRC - NOTE: don't use a 4.0, it won't work - plus a MicroUSB cable
* A 12V->5V Step Down Converter. Quality is important. I use an [Adafruit Adjustable Breadboard](https://www.adafruit.com/product/184)
* 2x [Molex 44133-1000](https://www.mouser.com/ProductDetail/538-44133-1000)
* 1x [Molex 43030-0001 cut strip of 100](https://www.mouser.com/ProductDetail/538-43030-0001-CT) (Cheapest way)
* Appropriate Molex Crimp Tool. Real Molex, not a knockoff from China. PWM stability is important.
* Pin headers for the Teensy are recommended, but you can go direct if you prefer
* 20ga and 22ga wire in a mix of colors, but you can do all one color if you really want
* Some 3.3V LEDs (totally optional)
* An ICXFAN10-MOD you can hack up, or you can order a 3D printed 'insert' module once I finish the design

Completely Optional Things:

* An ESP8266 if you want to have the WiFi status page (code not done yet)
* A MicroUSB cable you can cut up if you want to use the Teensy as a serial console (code not done yet)
* A MicroSD card for the Teensy comes in handy, or PSRAM, but it's absolutely not required

Important Notes:

* If you intend to use the ESP8266 WiFi, then you need the ACRYLIC top or to locate it outside the chassis. The ICX is EMI/RFI shielded.
* The Teensy 4.1 can be run at 5V or 3.3V, but 5V is recommended to reduce the load on the step-down converter.
* YOU MUST RUN THE TEENSY AT NO LESS THAN 450MHz! It's powerful, yes, but this mod is very sensitive to clocks.
* You can totally add RTC if you want. I'm not your supervisor.