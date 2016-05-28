#include "demo_LEDs.h"

void animateLEDs(){
  
  // moves a line of 10 LEDs across the strips
  int lineLength = 10;
  // where is the front of the line of lit LEDs on the strip?
  int linePosition = ticks % (LEDS_PER_STRIP + lineLength);
  // what color should the line be?
  Color lineColor = blue; // or rgbToColor(0, 0, 255)
  
  // go through every LED on both strips
  for (int i = 0; i < LEDS_PER_STRIP; i++) {
    // if the current LED is in the line, it should be turned on
    if(i < linePosition && i > linePosition - lineLength){
      setLED(i, lineColor);
    } else {
      setLED(i, black);
    }
  }
}

