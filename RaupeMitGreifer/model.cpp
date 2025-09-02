#include <Arduino.h>
#include "model.h"

const int hubSpeed = 250;
const int drehSpeed = 250;
const int greiferSpeed = 250;

RgbMatrix leds;
Ps4Controller controller;
Hub hub;
MotorSC leftMotor;
MotorSC rightMotor;
GreiferDrehung greiferDrehung;
Greifer greifer;

void onRemote(Ps4Controller* c) {
  // Hub
  GamePad pad = leftPad();
  if (pad.up) {
    hub.up(); 
  } else if (pad.down) {
    hub.down();
  } else {
    hub.stop();
  }

  // Greifer Drehung
  if (pad.left) {
    greiferDrehung.left(); 
  } else if (pad.right) {
    greiferDrehung.right();
  } else {
    greiferDrehung.stop();
  }

 // Greifer
  pad = rightPad();
  if (pad.right) {
    greifer.close(); 
  } else if (pad.left) {
    greifer.open();
  } else {
    greifer.stop();
  }
 
  // Raupen
  int r = rightStick().y;
  int l = leftStick().y;
  if (abs(r) < 20) rightMotor.stop(); else rightMotor.speed(r);
  if (abs(l) < 20) leftMotor.stop(); else leftMotor.speed(-l);
}

void initModel() {
  controller.setCallback(onRemote);
}

void Hub::up() {
  if (state != States::UP) {
    Serial.println("up");
    motor.gotoPos(600, hubSpeed);
    state = States::UP;
  }
}

void Hub::down() {
  if (state != States::DOWN) {
    Serial.println("down");
    motor.gotoPos(33, hubSpeed);
    state = States::DOWN;
  }
}

void Hub::stop() {
  if (state != States::STOP) {
    Serial.println("stop");
    motor.stop();
    state = States::STOP;
  }
}

void Hub::loop() {
  motor.loop();
  int pos = motor.servo->readPos(false);
  if (pos != oldPos) {
      if (state == States::UP) {
        if (pos >= 590) controller.rumble(200, 60, 100);
      }
      if (state == States::DOWN) {
        if (pos <= 35) controller.rumble(200, 60, 100);
      }
      oldPos = pos;
  }
}

void Hub::setup(u8 id) {
  state = States::UNDEFINED;
  motor.setup(id);
  motor.stop();
}


/////////////////////////////////////
void GreiferDrehung::left() {
  if (state != States::LEFT) {
    Serial.println("left");
    motor.gotoPos(15, drehSpeed);
    state = States::LEFT;
  }
}

void GreiferDrehung::right() {
  if (state != States::RIGHT) {
    Serial.println("right");
    motor.gotoPos(540, drehSpeed);
    state = States::RIGHT;
  }
}

void GreiferDrehung::stop() {
  if (state != States::STOP) {
    Serial.println("stop");
    motor.stop();
    state = States::STOP;
  }
}

void GreiferDrehung::loop() {
  motor.loop();
  int pos = motor.servo->readPos(false);
  if (pos != oldPos) {
      if (state == States::RIGHT) {
        if (pos >= 530) controller.rumble(200, 60, 100);
      }
      if (state == States::LEFT) {
        if (pos <= 30) controller.rumble(200, 60, 100);
      }
      oldPos = pos;
  }
}

void GreiferDrehung::setup(u8 id) {
  state = States::UNDEFINED;
  motor.setup(id);
  motor.stop();
}

/////////////////////////////////////
void Greifer::close() {
  if (state != States::CLOSE) {
    Serial.println("close");
    motor.gotoPos(809, greiferSpeed);
    state = States::CLOSE;
  }
}

void Greifer::open() {
  if (state != States::OPEN) {
    Serial.println("open");
    motor.gotoPos(300, greiferSpeed);
    state = States::OPEN;
  }
}

void Greifer::stop() {
  if (state != States::STOP) {
    Serial.println("stop");
    motor.stop();
    state = States::STOP;
  }
}

void Greifer::loop() {
  motor.loop();
  int pos = motor.servo->readPos(false);
  if (pos != oldPos) {
      if (state == States::CLOSE) {
        if (pos >= 800) controller.rumble(200, 60, 100);
      }
      if (state == States::OPEN) {
        if (pos <= 300) controller.rumble(200, 60, 100);
      }
      oldPos = pos;
  }
}

void Greifer::setup(u8 id) {
  state = States::UNDEFINED;
  motor.setup(id);
  motor.stop();
}



