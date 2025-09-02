#include <Arduino.h>
#include "arduino_multitasking.h"
#include "model.h"

void setup() {
  Serial.begin(115200);
  while(!Serial) {}

  controller.setup();
  controller.setInterval(20);
  scheduler.add(controller);

  leftMotor.setup(1);
  rightMotor.setup(2);
  scheduler.add(leftMotor);
  scheduler.add(rightMotor);

  hub.setup(3);
  scheduler.add(hub);

  greiferDrehung.setup(4);
  scheduler.add(greiferDrehung);

  greifer.setup(5);
  scheduler.add(greifer);

  initModel();

  // nur zum Spass
  leds.setup(5);
  leds.setInterval(5);
  scheduler.add(leds);
  leds.rainbow(20);
  leds.show();
}

void loop() {
  scheduler.loop();
}

