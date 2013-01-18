This is a library for the Adafruit 2.8" TFT Display Shield with Touchscreen.
  ----> http://www.adafruit.com/products/376

It is an extension on the following Adafruit libraries:

https://github.com/adafruit/Touch-Screen-Library
https://github.com/adafruit/TFTLCD-Library
https://github.com/adafruit/Adafruit-GFX-Library

They must be installed before this library can be used. This library is intended only for the SHIELD with the control lines fixed by the hardware. It would need extension to work with different pins.

It creates a single class RWS_TFTLCD_TS that includes both the display and the touch screen, allowing the class to know the current rotation of the display in order to translate the touchscreen coordinates into display coordinates.

DISCLAIMER:
This Library is NOT provided by AdaFruit and they have not
endorsed it. The use of the Adafruit name is merely to provide
a reference to which devices/libraries this library works with
and depends on. It is in no way intended to convey any affiliation
whatsoever with Adafruit or imply that Adafruit supports this
library in any way.

Written by Rick Sellens, first commit January 18, 2012
This work is licensed under a Creative Commons Attribution-ShareAlike 3.0 Unported License
 