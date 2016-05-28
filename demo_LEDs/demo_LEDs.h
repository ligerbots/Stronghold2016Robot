// this file handles talking to the LEDs and calls animateLEDs
#include <Adafruit_NeoPixel.h>

// constants
#define PIN_LEFT 4
#define PIN_RIGHT 6
#define LEDS_PER_STRIP 23
#define LEDS_MODE NEO_GRB+NEO_KHZ800

typedef uint32_t Color;

// variables
Adafruit_NeoPixel stripLeft(LEDS_PER_STRIP, PIN_LEFT, LEDS_MODE);
Adafruit_NeoPixel stripRight(LEDS_PER_STRIP, PIN_RIGHT, LEDS_MODE);
Color red;
Color orange;
Color yellow;
Color green;
Color blue;
Color violet;
Color black;
Color white;
int ticks = 0;

void animateLEDs();

void setup() {
  // LED strip setup
  stripLeft.begin();
  stripRight.begin();
  stripLeft.show();
  stripRight.show();
  red = stripLeft.Color(255, 0, 0);
  orange = stripLeft.Color(255, 165, 0);
  yellow = stripLeft.Color(255, 255, 0);
  green = stripLeft.Color(0, 255, 0);
  blue = stripLeft.Color(0, 0, 255);
  violet = stripLeft.Color(255, 0, 255);
  black = stripLeft.Color(0, 0, 0);
  white = stripLeft.Color(255, 255, 255);

  Serial.begin(9600);
  Serial.println("Completed setup()");
}

void loop() {
  ticks++;

  animateLEDs();
  
  stripLeft.show();
  stripRight.show();
  delay(20);
}

Color rgbToColor(int r, int g, int b){
  return stripLeft.Color(r, g, b);
}

void setLED(int which, int color){
  stripLeft.setPixelColor(which, color);
  stripRight.setPixelColor(which, color);
}

void animate_fade(Color color){
  int r = (color >> 16) & 0xFF;
  int g = (color >> 8) & 0xFF;
  int b = color & 0xFF;
  
  int val_r = (int) (((double) abs((ticks % 50) - 25)) * r / 25.0);
  int val_g = (int) (((double) abs((ticks % 50) - 25)) * g / 25.0);
  int val_b = (int) (((double) abs((ticks % 50) - 25)) * b / 25.0);
  color = stripLeft.Color(val_r, val_g, val_b);
  for (int i = 0; i < LEDS_PER_STRIP; i++) {
    stripLeft.setPixelColor(i, color);
    stripRight.setPixelColor(i, color);
  }
}

void animate_all_off() {
  for (int i = 0; i < LEDS_PER_STRIP; i++) {
    stripLeft.setPixelColor(i, black);
    stripRight.setPixelColor(i, black);
  }
}
