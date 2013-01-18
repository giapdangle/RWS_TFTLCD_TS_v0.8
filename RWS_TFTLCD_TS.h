// This is a library for the Adafruit 2.8" TFT Display Shield with Touchscreen.
//   ----> http://www.adafruit.com/products/376
//
// Extension to Adafruit_TFTLCD and TouchScreen by Rick Sellens CC license
// See README.txt

#ifndef _RWS_TFTLCD_TS_H_
#define _RWS_TFTLCD_TS_H_

#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>

class RWS_TFTLCD_TS : public Adafruit_TFTLCD, public TouchScreen {

 public:

  RWS_TFTLCD_TS(void);
  RWS_TFTLCD_TS(uint16_t rx);

// set the calibration points for the touchscreen values at TFT screen edges
// get values by running the example program and using a stylus  
  void     setLimits(uint16_t minX, uint16_t minY, uint16_t maxX, uint16_t maxY);

// get a Touch Screen point in TFT coordinates (e.g. 0-319 and 0-239) corrected for current rotation
  Point    getPointTFT(void);

// draw a button on the TFT screen - see the example program
  void     drawButton(int16_t x, int16_t y, int16_t sx, int16_t sy, uint16_t color, uint16_t tcolor, 
                 const char *t1, const char *t2, const char *t3, int16_t shade, int16_t dly);
  
// create a chart recorder region on the screen - see the example program  
  void     setCR(uint8_t n, uint16_t *colors, uint16_t startX, uint16_t startY, uint16_t sX, uint16_t sY, uint16_t jumpX);
  void     setCR(uint8_t n, uint16_t *colors, uint16_t startX, uint16_t startY, uint16_t sX, uint16_t sY);
  void     clrCR(void);                      // clear the space
  void     leftCR(void);                     // start again at the left side
  uint16_t drawNextCR(uint16_t *newVals);    // draw the next line segment, newVals scaled by caller to sY 


 private:

  void     setDefaults();

  uint8_t  _yp, _ym, _xm, _xp;                    // the touchscreen pins
  uint16_t _TSminX, _TSminY, _TSmaxX, _TSmaxY;   // the touchscreen values matching TFT edges
  uint16_t _CRn,_CRcolors[8], _CRbg;             // the number of chart recorder traces and their colors / background
  uint16_t _CRstartX, _CRsX, _CRjumpX;           // starting X pixels and size, pixels per step
  uint16_t _CRstartY, _CRsY;
  uint16_t _CRscrCurrent;

};

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GRAY    0x7777

// make any 16 bit RGB color from component values between 0 and 63
#define RGB(red,green,blue)     ( (unsigned) (((red &      0b111111) >> 1) << 11) | \
                                              ((green &    0b111111) << 5) | \
                                              ((blue &     0b111111) >> 1)  )

// decompose to component values between 0 and 63
#define redComp(rgb)            ((((unsigned) rgb >> 11) & 0b011111) << 1)   // MS 5 bits         
#define greenComp(rgb)          (((unsigned) rgb >> 5) &   0b111111)         // middle 6 bits
#define blueComp(rgb)           (((unsigned) rgb &         0b011111) << 1)   // LS 5 bits


#endif