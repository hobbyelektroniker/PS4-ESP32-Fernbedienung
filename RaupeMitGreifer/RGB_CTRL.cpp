
#include "RGB_CTRL.h"

Adafruit_NeoPixel matrix = Adafruit_NeoPixel(NUMPIXELS, RGB_LED, NEO_GRB + NEO_KHZ800);

enum RgbStates {
  STOP, WIPE, RAINBOW, 
};

uint32_t rgbColor(byte r, byte g, byte b) {
  return matrix.Color(r, g, b);
}

void RgbMatrix::show() {
  matrix.show();
}

void RgbMatrix::setColor(int led, uint32_t color) {
  state = STOP;
  matrix.setPixelColor(led, color);
}

void RgbMatrix::off() {
  state = STOP;
  for (int i=0; i<NUM_LEDS; i++) {
    setColor(i, matrix.Color(0, 0, 0));
  }
  show();
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t RgbMatrix::wheelColor(byte wheelPos) {
  if(wheelPos < 85) {
    return matrix.Color(wheelPos * 3, 255 - wheelPos * 3, 0);
  } 
  else if(wheelPos < 170) {
    wheelPos -= 85;
    return matrix.Color(255 - wheelPos * 3, 0, wheelPos * 3);
  } 
  else {
    wheelPos -= 170;
    return matrix.Color(0, wheelPos * 3, 255 - wheelPos * 3);
  }
}

void RgbMatrix::rainbow(uint8_t waitTime) {
  rainbowTime = waitTime;
  state = RAINBOW;
  rainbowStep = 0; 
}

void RgbMatrix::setup(int brightness) {
  matrix.setBrightness(brightness);
  matrix.begin();
  matrix.show();
}

void RgbMatrix::loop() {
  switch (state) {
    case RAINBOW:
      uint16_t i, j;
      for(i=0; i<matrix.numPixels(); i++) {
        matrix.setPixelColor(i, wheelColor((125*i+rainbowStep) & 255));
      }
      show();
      wait(rainbowTime);
      rainbowStep += 1;
      if (rainbowStep >= 250) {
        rainbowStep = 5;
      }
  }
}

