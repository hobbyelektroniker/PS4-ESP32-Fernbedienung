
// WS2812:
#pragma once
#include <Adafruit_NeoPixel.h>
#include "arduino_multitasking.h"

#define RGB_LED   23
#define NUMPIXELS 2

uint32_t rgbColor(byte r, byte g, byte b);

class RgbMatrix : public Component {
  public:
    int NUM_LEDS = NUMPIXELS;
    int RAINBOW_STATUS = 0;

    unsigned long rainbowTime;
    int pixelNum;
    int rainbowStep;

    void show();
    void setColor(int led, uint32_t color);
    void off();
    uint32_t wheelColor(byte wheelPos);
    void rainbow(uint8_t waitTime);
    void setup(int brightness);
    void loop();
  };