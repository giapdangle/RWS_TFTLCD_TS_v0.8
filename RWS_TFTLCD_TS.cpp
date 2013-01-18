// This is a library for the Adafruit 2.8" TFT Display Shield with Touchscreen.
//   ----> http://www.adafruit.com/products/376
//
// Extension to Adafruit_TFTLCD and TouchScreen by Rick Sellens CC license
// See README.txt

#include "RWS_TFTLCD_TS.h"

// Assumes the Shield pins are used
RWS_TFTLCD_TS::RWS_TFTLCD_TS(void) 
  : TouchScreen(6, A1, A2, 7),
    Adafruit_TFTLCD()
{
  setDefaults();
}

RWS_TFTLCD_TS::RWS_TFTLCD_TS(uint16_t rx) 
  : TouchScreen(6, A1, A2, 7, rx),
    Adafruit_TFTLCD()
{
  setDefaults();
}

void RWS_TFTLCD_TS::setDefaults(void) 
{
  _yp = A1;
  _xm = A2;
  _ym = 7;
  _xp = 6;

  _TSminX = 185;
  _TSminY = 135;
  _TSmaxX = 880;
  _TSmaxY = 940;

  _CRn = 1;
  _CRcolors[0] = WHITE;
  _CRbg = BLACK;
  _CRstartX = 0;
  _CRsX = width();
  _CRstartY = 0;
  _CRsY = height();
  _CRjumpX = 1;
  _CRscrCurrent = _CRstartX;
}

//--------------  T F T   C A L I B R A T E D   T O U C H S C R E E N   P O I N T S  -------

void RWS_TFTLCD_TS::setLimits(uint16_t minX, uint16_t minY, uint16_t maxX, uint16_t maxY)
{
  _TSminX = minX;
  _TSminY = minY;
  _TSmaxX = max(maxX,minX+1);
  _TSmaxY = max(maxY,minY+1);
}  

Point RWS_TFTLCD_TS::getPointTFT(){ // returns a touch screen point in TFTLCD coords
  Point p = getPoint();
  
  if (p.z > 10) {
    Point p1 = p;
    switch(rotation){
      case 0:
        // this one matches mode 0, origin top left, portrait, usb top
        p.x = map(p1.x, _TSminX, _TSmaxX, _width, 0);
        p.y = map(p1.y, _TSminY, _TSmaxY, _height, 0);
        break;
      case 1:
        // this one matches mode 1, origin top left, landscape, usb left
        p.x = map(p1.y, _TSminY, _TSmaxY, _width, 0);
        p.y = map(p1.x, _TSminX, _TSmaxX, 0, _height);
        break;
      case 2:
        // this one matches mode 2, origin top left, portrait, usb bottom
        p.x = map(p1.x, _TSminX, _TSmaxX, 0, _width);
        p.y = map(p1.y, _TSminY, _TSmaxY, 0, _height);
        break;
      case 3:
        // this one matches mode 3, origin top left, landscape, usb right
        p.x = map(p1.y, _TSminY, _TSmaxY, 0, _width);
        p.y = map(p1.x, _TSminX, _TSmaxX, _height, 0);
        break;
    }    
  }
  else{          // null out the point if out of range
    p.x = 0;
    p.y = 0;
    p.z = 0;
  }

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  // This cleanup was in the original Adafruit Paint example
  // for the shield configuration, but it doesn't seem to stick
  pinMode(_xp, OUTPUT);
  pinMode(_xm, OUTPUT);
  pinMode(_yp, OUTPUT);
  pinMode(_ym, OUTPUT);

  return p;
}

//----------------------------  B U T T O N S  ---------------------------------

void RWS_TFTLCD_TS::drawButton(int16_t x, int16_t y, int16_t sx, int16_t sy, uint16_t color, uint16_t tcolor, const char *t1, const char *t2, const char *t3, int16_t shade, int16_t dly)
{
  // buttons 30 pixels high are good with a single row of text 
  float scale = 1.0, fade = 1.0;
  uint16_t red, green, blue;

  //draw the button in the base color
  fillRect(x,y,sx,sy,color);

  //shade the button edges
  scale = pow(0.995,shade);
  for(int16_t i = sy/4;i >= 0;i--){
    fade *= scale;
    red = (fade * redComp(color));
    if(red > 255)  red = 0; else if(red > 62) red = 62;
    green = (fade * (greenComp(color)));
    if(green > 255) green = 0; else if(green > 63) green = 63;
    blue = (fade * blueComp(color));
    if(blue > 255) blue = 0; else if(blue > 62) blue = 62;
    unsigned rgb = RGB(red,green,blue);  
    drawRect(x+i, y+i, sx-2*i, sy-2*i, rgb);
    delay(dly);    // slow down for effect
  }
  
  // print text on the button
  setTextSize(2);            // pixels per dot in letter
  setTextColor(tcolor);
  setCursor(x+10,y+8);       // position of upper left corner of letter
  print(t1);
  setTextSize(1);
  setCursor(x+10,y+25);
  print(t2);
  setCursor(x+10,y+36);
  print(t3);
}

//-----------------  C H A R T   R E C O R D E R  -------------------------------

void     RWS_TFTLCD_TS::setCR(uint8_t n, uint16_t *colors, uint16_t startX, uint16_t startY, uint16_t sX, uint16_t sY, uint16_t jumpX = 1)
{
  // no more than 8 plotted lines hard coded in
  _CRn = max(n,1);
  _CRn = min(_CRn,8);

  // the first n colors are the line colors for the plotted variables and the next color is the background
  for(uint8_t i = 0;i < _CRn;i++) _CRcolors[i] = colors[i];
  _CRbg = colors[n];

  // X and Y have to start and finish on the display and be at least 10 pixels
  _CRstartX = min(startX,(uint16_t) width()-10);   // at least 10 back from the edge
  _CRsX = min(sX,(uint16_t) width()-_CRstartX);    // no more than the distance to the edge
  _CRsX = max(_CRsX,10);                           // no less than 10
  _CRstartY = min(startY,(uint16_t) height()-10);
  _CRsY = min(sY,(uint16_t) height()-_CRstartY);
  _CRsY = max(_CRsY,10);

  _CRjumpX = min(jumpX,_CRsX / 3);                 // jump less than one third the display size

  clrCR();                                         // clear the space and draw the background
}

void     RWS_TFTLCD_TS::setCR(uint8_t n, uint16_t *colors, uint16_t startX, uint16_t startY, uint16_t sX, uint16_t sY)
{
  setCR(n,colors,startX,startY,sX,sY,1);
}

void     RWS_TFTLCD_TS::clrCR(void)
{
  fillRect(_CRstartX,_CRstartY,_CRsX,_CRsY,_CRbg);
  _CRscrCurrent = _CRstartX;
}

void     RWS_TFTLCD_TS::leftCR(void)
{
  if(_CRscrCurrent < _CRstartX + _CRsX - 8) fillRect(_CRscrCurrent - _CRjumpX, _CRstartY, 9+_CRjumpX, _CRsY, _CRbg);
  _CRscrCurrent = _CRstartX;
}

uint16_t RWS_TFTLCD_TS::drawNextCR(uint16_t *newVals)
{
  static uint16_t oldVals[] = {0,0,0,0,0,0,0,0};   // this must include at least n positions
  
  if(_CRscrCurrent >= _CRstartX+_CRsX) _CRscrCurrent = _CRstartX;
  //if(_CRscrCurrent > _CRstartX){
    int j = _CRscrCurrent; 
    int j0 = j - _CRjumpX;
    if(_CRscrCurrent < _CRstartX + _CRsX  - 11) fillRect(j0+10+_CRjumpX, _CRstartY, 1, _CRsY, YELLOW);
    if(_CRscrCurrent < _CRstartX + _CRsX - 8) fillRect(j0+1, _CRstartY, 9+_CRjumpX, _CRsY, _CRbg);
    for(uint8_t chNo=0;chNo < _CRn;chNo++){
      unsigned nv = min(newVals[chNo],_CRsY);   // make it fit in the window
      if(_CRscrCurrent > _CRstartX)             // only draw if we're into the window
        // invert the sense of y so that increasing y is upwards when you draw the line
        drawLine(j0, _CRstartY + (_CRsY - oldVals[chNo]),j, _CRstartY + (_CRsY - nv),_CRcolors[chNo]);
      oldVals[chNo] = nv;                       // remember the plotted value
    }
  //}
  _CRscrCurrent += _CRjumpX;
  return _CRscrCurrent;
}

