#include <Wire.h>
#include <stdint.h>

//#include "LPD8806.h"
#include "SPI.h"

// Example to control LPD8806-based RGB LED Modules in a strip

/*****************************************************************************/

// Number of RGB LEDs in strand:
int nLEDs = 20;

// Chose 2 pins for output; can be any valid output pins:
int dataBlue  = 7;
int clockYellow = 6;


//LPD8806 strip = LPD8806(nLEDs, dataBlue, clockYellow);

enum Pattern {DISTANCE_OFF,CLOSE , MIDDLE, FAR, SHOOTER_OFF, UNLOADED, LOADING, FIRING};

void setup() {
  
//    strip.begin();
//     
//   strip.show(); // register event 
   Wire.begin(2);                // join i2c bus with address #2 
    Wire.onReceive(onReceivedData);
    Wire.onRequest(onRequestData);
  Serial.begin(9600); 
}

void onReceivedData(int byteCount)
{
  
Serial.print("Received data!");
Serial.println();
  
}

void onRequestData()
{
  
Serial.print("Requested data!");
Serial.println();
  
}
void loop() 
{
 delay(100);
 Serial.println("Speed");
 
}

//void loadingPattern(){
//  for (int i =9; i<20;i++)
//  {
//    strip.setPixelColor(i,strip.Color(255,0,255));
//    strip.setPixelColor(i+10,strip.Color(255,0,255));
//  }
//  for (int i =44; i<50;i++)
//  {
//     strip.setPixelColor(i,strip.Color(255,0,255));
//     strip.setPixelColor(i+10,strip.Color(255,0,255));
//  }
//colorChase(strip.Color(255,255,0),50);
//   
//}
//void unloadedPattern(){
//  for (int i =9; i<20;i++)
//  {
//    strip.setPixelColor(i,strip.Color(0,0,255));
//    strip.setPixelColor(i+10,strip.Color(0,0,255));
//  }
//  for (int i =44; i<50;i++)
//  {
//     strip.setPixelColor(i,strip.Color(0,0,255));
//     strip.setPixelColor(i+10,strip.Color(0,0,255));
//  }
//}
//void firingPattern(){
//  for (int i =9; i<20;i++)
//  {
//    strip.setPixelColor(i,strip.Color(255,0,0));
//    strip.setPixelColor(i+10,strip.Color(255,0,0));
//  }
//  for (int i =44; i<50;i++)
//  {
//     strip.setPixelColor(i,strip.Color(255,0,0));
//     strip.setPixelColor(i+10,strip.Color(255,0,0));
//  }
//}
//void shooterOff(){
//
//  for (int i =9; i<20;i++)
//  {
//    strip.setPixelColor(i,0);
//    strip.setPixelColor(i+10,0);
//  }
//  for (int i =44; i<50;i++)
//  {
//     strip.setPixelColor(i,0);
//     strip.setPixelColor(i+10,0);
//  }
//  
//}
//void distanceOff(){
//    for (int i =0; i<10;i++)
//  {
//    strip.setPixelColor(i,0);
//    strip.setPixelColor(i+10,0);
//  }
//  for (int i =39; i<45;i++)
//  {
//     strip.setPixelColor(i,0);
//     strip.setPixelColor(i+10,0);
//  }
//  
//}
//void farPattern(){
//  for (int i =0; i<10;i++)
//  {
//    strip.setPixelColor(i,0);
//    strip.setPixelColor(i+10,0);
//  }
//  for (int i =39; i<45;i++)
//  {
//     strip.setPixelColor(i,0);
//     strip.setPixelColor(i+10,0);
//  }
//}
//void middlePattern(){
//  for (int i =0; i<10;i++)
//  {
//    strip.setPixelColor(i,0);
//    strip.setPixelColor(i+10,0);
//  }
//  for (int i =39; i<45;i++)
//  {
//     strip.setPixelColor(i,0);
//     strip.setPixelColor(i+10,0);
//  }
//}
//void closePattern(){
//  for (int i =0; i<10;i++)
//  {
//    strip.setPixelColor(i,0);
//    strip.setPixelColor(i+10,0);
//  }
//  for (int i =39; i<45;i++)
//  {
//     strip.setPixelColor(i,0);
//     strip.setPixelColor(i+10,0);
//  }
//}
//void rainbow(uint8_t wait) {
//  int i, j;
//   
//  for (j=0; j < 384; j++) {     // 3 cycles of all 384 colors in the wheel
//    for (i=0; i < strip.numPixels(); i++) {
//      strip.setPixelColor(i, Wheel( (i + j) % 384));
//    }  
//    strip.show();   // write all the pixels out
//    delay(wait);
//  }
//}
//
//// Slightly different, this one makes the rainbow wheel equally distributed 
//// along the chain
//void rainbowCycle(uint8_t wait) {
//  uint16_t i, j;
//  
//  for (j=0; j < 384 * 5; j++) {     // 5 cycles of all 384 colors in the wheel
//    for (i=0; i < strip.numPixels(); i++) {
//      // tricky math! we use each pixel as a fraction of the full 384-color wheel
//      // (thats the i / strip.numPixels() part)
//      // Then add in j which makes the colors go around per pixel
//      // the % 384 is to make the wheel cycle around
//      strip.setPixelColor(i, Wheel( ((i * 384 / strip.numPixels()) + j) % 384) );
//    }  
//    strip.show();   // write all the pixels out
//    delay(wait);
//  }
//}
//
//// Fill the dots progressively along the strip.
//void colorWipe(uint32_t c, uint8_t wait) {
//  int i;
//
//  for (i=0; i < strip.numPixels(); i++) {
//      strip.setPixelColor(i, c);
//      strip.show();
//      delay(wait);
//  }
//}
//
//// Chase one dot down the full strip.
//void colorChase(uint32_t c, uint8_t wait) {
//  int i;
//
//  // Start by turning all pixels off:
//  for(i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);
//
//  // Then display one pixel at a time:
//  for(i=0; i<strip.numPixels(); i++) {
//    strip.setPixelColor(i, c); // Set new pixel 'on'
//    strip.show();              // Refresh LED states
//    strip.setPixelColor(i, 0); // Erase pixel, but don't refresh!
//    delay(wait);
//  }
//
//  strip.show(); // Refresh to turn off last pixel
//}
//
////Theatre-style crawling lights.
//void theaterChase(uint32_t c, uint8_t wait) {
//  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
//    for (int q=0; q < 3; q++) {
//      for (int i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, c);    //turn every third pixel on
//      }
//      strip.show();
//     
//      delay(wait);
//     
//      for (int i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, 0);        //turn every third pixel off
//      }
//    }
//  }
//}
//
////Theatre-style crawling lights with rainbow effect
//void theaterChaseRainbow(uint8_t wait) {
//  for (int j=0; j < 384; j++) {     // cycle all 384 colors in the wheel
//    for (int q=0; q < 3; q++) {
//        for (int i=0; i < strip.numPixels(); i=i+3) {
//          strip.setPixelColor(i+q, Wheel( (i+j) % 384));    //turn every third pixel on
//        }
//        strip.show();
//       
//        delay(wait);
//       
//        for (int i=0; i < strip.numPixels(); i=i+3) {
//          strip.setPixelColor(i+q, 0);        //turn every third pixel off
//        }
//    }
//  }
//}
///* Helper functions */
//
////Input a value 0 to 384 to get a color value.
////The colours are a transition r - g -b - back to r
//
//uint32_t Wheel(uint16_t WheelPos)
//{
//  byte r, g, b;
//  switch(WheelPos / 128)
//  {
//    case 0:
//      r = 127 - WheelPos % 128;   //Red down
//      g = WheelPos % 128;      // Green up
//      b = 0;                  //blue off
//      break; 
//    case 1:
//      g = 127 - WheelPos % 128;  //green down
//      b = WheelPos % 128;      //blue up
//      r = 0;                  //red off
//      break; 
//    case 2:
//      b = 127 - WheelPos % 128;  //blue down 
//      r = WheelPos % 128;      //red up
//      g = 0;                  //green off
//      break; 
//  }
//  return(strip.Color(r,g,b));
//}
