// Example program for the RWS_TFTLCD_TS library without write access to the SD card, saving about 10K 
//
// Written by Rick Sellens CC license.
// see README.txt
//
// This program started life as an Adafruit Paint example specifically for the TFTLCD Arduino shield.
// It is really sensitive to size, so you have to pick features to use - eliminating sprintf() saves about 1500 bytes.
// Defining short, repetitively used strings as variables doesn't save space, possibly function call overhead?
// Using the smallest possible data type also cuts much more than just the storage size.
// Just getting the code in doesn't mean there won't be a crash for no good reason (stack overflow?)

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // TFTLCD Hardware-specific library
#include <TouchScreen.h>     // TouchScreen library
#include <RWS_TFTLCD_TS.h>   // Combined class library inherits from all three

// These are the touch screen pins for the shield!
// You need to know them if you want to use getPoint(), then fix the pins
// getPointTFT() is supposed to do the fixup automatically, but that isn't working for me.
#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin

// Measure the touch screen minimum and maximum values by sliding a stylus to 
// the edges of the illuminated part of the TFT. The touchscreen return values 
// will extend further in range out into the unilluminated edge.
// These values should be taken in portrait mode for Y vertical and X horizontal
#define TS_MINX 185
#define TS_MINY 135
#define TS_MAXX 880
#define TS_MAXY 940

// --------------  G L O B A L   V A R I A B L E S   A S S U M E D  ----------------------- 
// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
// RWS_TFTLCD_TS tftts(300) will call TouchScreen(XP, YP, XM, YM, 300);
// doesn't seem to be sensitive to the change from 300 to 338 on mine
RWS_TFTLCD_TS tftts(338);

//----------------------- S E T U P  ----------------------------------------------

void setup(void) {
  Serial.begin(57600);
  Serial.println("TFTLCD TS!");
  tftts.reset();
  uint16_t identifier = tftts.readID();
  if(identifier == 0x9325) Serial.println("ILI9325 LCD");
  else Serial.println("TFT failed");
  tftts.begin(identifier);
  tftts.setRotation(1);
  tftts.setLimits(TS_MINX,TS_MINY,TS_MAXX,TS_MAXY);
  tftts.fillScreen(BLACK);

  //Put splash screen code here

}

//------------------------- L O O P -------------------------------------
void loop()
{
  
  buttonExamples();
  
  uint16_t rgb = BLUE;
  rgb = pickAColor(rgb);
  Serial.print("You picked RGB ");
  Serial.println(rgb);
  
  findEdges(rgb);
  }

void findEdges(uint16_t c1){
  uint16_t colors[] = {RED,GREEN,YELLOW,BLACK};
  uint16_t vals[] = {0,0,0};
  
  unsigned long startTime = millis();
  tftts.fillScreen(WHITE);
  tftts.setTextSize(2);
  tftts.setTextColor(BLACK,WHITE);
  tftts.setCursor(10,10);
  tftts.print("    30 seconds to find");
  tftts.setCursor(10,40);
  tftts.print("  the edges in TS coords");
  tftts.setCursor(10,120);
  tftts.print("Use values in setLimits()");
  colors[3] = c1;
  tftts.setCR(3,colors,30,140,260,80);
  uint16_t i=0;
  while(millis() - startTime < 30000){
    i++;
    Point p = tftts.getPoint();
    // make sure to switch these pins back to output mode
    // the library was supposed to but it isn't happening
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    if(p.z != 0){
      tftts.setCursor(10,80);
      tftts.print("   X= "); tftts.print(p.x); tftts.print(" Y= "); tftts.print(p.y); 
      tftts.print(" Z= "); tftts.print(p.z); tftSp();tftSp();
    //vals[0]= p.x/12;
    //vals[1]= p.y/12;
    //vals[2]= p.z/6;
    //tftts.drawNextCR(vals);
    }
    // not using trig in the sample curves cuts out a chunk of program size
    vals[0]= sin( 1+((float) i / 10.0)) * 20 + 40;
    vals[1]= sin( 2+((float) i / 20.0)) * 30 + 40;
    vals[2]= sin( 3+((float) i / 10.0)) * 20 + 40;
    tftts.drawNextCR(vals);
  }
}

void buttonExamples(){
  tftts.fillScreen(BLACK);
  oneButton(0,GREEN,RED,0,0);
  oneButton(1,RED,GREEN,2,20);
  oneButton(2,GREEN,RED,4,40);
  oneButton(3,RED,GREEN,6,60);
}

void oneButton(int8_t rot, uint16_t c1, uint16_t c2, int8_t shd, int8_t dly){
  tftts.setRotation(rot);
  tftts.drawButton(20, 20, 90,90, c1, c2, "Button","Touch","Now",shd,dly);
  Point p = tftts.getPointTFT(); 
  while(p.z == 0) p = tftts.getPointTFT();
  tftts.fillCircle(p.x, p.y, 6, BLACK);
  tftts.fillCircle(p.x, p.y, 3, RED);  
  
}

uint16_t pickAColor(uint16_t currentColor){
  //uint16_t currentColor = BLUE; 
  uint16_t textColor = WHITE;
  int8_t red = 0,green = 0,blue = 0,shade = 2;
  static boolean first = 1;
  
  tftts.setRotation(1);
  tftts.fillScreen(BLACK);
 
  tftts.drawButton(240,  0, 80, 30, RED,           WHITE, " +", " "," ", 3,5);
  tftts.drawButton(240, 30, 80, 30, RGB(45,0,0),   WHITE, " -", " "," ",-3,5);
  tftts.drawButton(240, 60, 80, 30, GREEN,         RED,   " +", " "," ", 3,5);
  tftts.drawButton(240, 90, 80, 30, RGB(0,45,0),   WHITE, " -", " "," ",-3,5);
  tftts.drawButton(240,120, 80, 30, BLUE,          YELLOW," +", " "," ", 3,5);
  tftts.drawButton(240,150, 80, 30, RGB(0,0,45),   YELLOW," -", " "," ",-3,5);
  tftts.drawButton(240,210, 80, 30, YELLOW,        BLUE,"DONE", " "," ", 3,5);
  tftts.drawButton(  0, 90, 80, 60, WHITE,         BLACK, " +", " "," ", 5,5);
  tftts.drawButton(  0,150, 80, 60, RGB(18,18,18), WHITE, " -", " "," ",-5,5);
  while(1){
    red = redComp(currentColor);  
    green = greenComp(currentColor);  
    blue = blueComp(currentColor);  
    Point p = tftts.getPointTFT();     // Get a touch input if it's there
    if ((p.z != 0) || first) {
      first = 0;
      if(p.x > 240){
        int8_t j = p.y / 30;
        switch(j){
          case 0: red+=2;   break;
          case 1: red-=2;   break;
          case 2: green++; break;
          case 3: green--; break;
          case 4: blue+=2;  break;
          case 5: blue-=2;  break;
          case 7: 
            tftts.fillScreen(BLACK);
            first = 1;
            return currentColor;
          default: break;
        }
      }
      if(p.x < 80){ 
        if(p.y > 150) shade--; else shade ++;
      }
      if(red < 0) red = 0; else if(red > 62) red = 62;  
      if(green < 0) green = 0; else if(green > 63) green = 63;  
      if(blue < 0) blue = 0; else if(blue > 62) blue = 62;  
      currentColor = RGB(red,green,blue); 
      if(red+green+blue > 100) textColor = BLACK; else textColor = WHITE;
      tftts.drawButton(110, 70, 100, 60, currentColor, textColor, "Line 1", "Line 2 longer", "Line 3 longest",shade,0);
      tftts.fillRect(110, 160, 100, 60, currentColor);
      tftts.fillRect(0,0,240,60,WHITE);
      tftts.setTextSize(2);        // pixels per dot in letter
      tftts.setCursor(3,4);      // position of upper left corner of letter
      tftts.setTextColor(BLACK,WHITE);
      tftts.print("Color / Shade = ");
      tftts.print(shade);
      tftts.setCursor(3,28);      // position of upper left corner of letter
      tftSp();
      tftts.print(currentColor);
      tftSp();
      tftts.setTextColor(RED,WHITE);
      tftts.print(red);
      tftSp();
      tftts.setTextColor(GREEN,WHITE);
      tftts.print(green);
      tftSp();
      tftts.setTextColor(BLUE,WHITE);
      tftts.print(blue);
    }
  }
}

void tftSp(){        // this saves just a few bytes, but at 31,854 / 32,256 used it all counts
  tftts.print(" ");
}
   
