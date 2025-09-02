#include <Arduino.h>
#include "servos.h"

void MotorSC::block() {servo->torque(true);}

void MotorSC::release() {
  stop();
  servo->torque(false);
}

void MotorSC::stop() {
  _speed = 0;
  servo->wheelSpeed(_speed);
  servo->torque(true);
}  

void MotorSC::targetReached() {
  _speed = _targetSpeed;
  _steps = 0;
  // Serial.printf("Ziel %d erreicht\n", _speed);
  // Callback
}

void MotorSC::setAccelTime(long accelTime) {
  _accelTime = accelTime;
}

bool MotorSC::isAccelerating() {
  return (_steps > 0);
}

void MotorSC::speed(int value) {
  int pwm = servo->maxPWM;
  _speed = constrain(value, -pwm, pwm);
  _steps = 0;
}

int MotorSC::speed() {
  return _speed;
}

void MotorSC::setSpeedTarget(int value, long accelTime) {
  int pwm = servo->maxPWM;
  _targetSpeed = constrain(value, -pwm, pwm);

  if (_targetSpeed == _speed) {
    targetReached();
    return;
  };
  if (accelTime != -1) _accelTime = accelTime;
  
  _steps = 1;
  if (_accelTime != 0) _steps = _accelTime / _interval;
  if (_steps < 1) {
    targetReached();
    return;
  }
  _speedStep = (_targetSpeed - _speed) / _steps;

  // Serial.printf("Geschwindigkeit %d -> %d, Schritte %d, Schritt %d\n", 
  //   _speed, _targetSpeed, _steps, _speedStep);
}

void MotorSC::setup(u8 id) {
  _id = id;
  _speed = _speedStep = _targetSpeed = _steps = 0;
  _accelTime = 1000;
  servo = new SCServo(id);
  servo->enableWheelMode();
  stop();
}



void MotorSC::loop() {
  // Serial.printf("Schritte %d, Speed %d, Schrittgrösse %d\n", _steps, _speed, _speedStep);
  if (_steps > 0) {
    _steps--;
    _speed += _speedStep;
    if (_steps == 0) {
      _speed = _targetSpeed;
      targetReached();
    }
  }
  servo->wheelSpeed(_speed);
}

/////////////////////////////////////
void ServoSC::block() {servo->torque(true);}
void ServoSC::release() {servo->torque(false);}
void ServoSC::stop() {servo->stopServo();}  

void ServoSC::targetReached() {
  // Serial.printf("Ziel %d erreicht\n", _speed);
  // Callback
}

void ServoSC::gotoAngle(float angle, int speed) {
  servo->gotoAngle(angle, speed);
}

void ServoSC::gotoPos(int pos, int speed) {
  servo->gotoPos(pos, speed);
}

void ServoSC::setup(u8 id) {
  _id = id;
  servo = new SCServo(id);
  stop();
}

void ServoSC::loop() {
  // Serial.printf("Schritte %d, Speed %d, Schrittgrösse %d\n", _steps, _speed, _speedStep);
}
