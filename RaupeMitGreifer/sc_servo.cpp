
#include "sc_servo.h"

SCSCL sc;

SCServo::SCServo(u8 servoId) {
  SCServo::init();
  _id = servoId;
  _angleGoal = -1;
}

void SCServo::init() {
  if (!Serial1) {
    Serial1.begin(1000000, SERIAL_8N1, S_RXD, S_TXD);
    while(!Serial1) {};
  }
  if (!sc.pSerial) sc.pSerial = &Serial1;
}

bool SCServo::ping(u8 id) {
  SCServo::init();
  return (sc.Ping(id) == id);
}

void SCServo::enableWheelMode() {
  _wheelEnabled = true;  
  sc.PWMMode(_id);
}

bool SCServo::wheelMode() {
  return _wheelEnabled; 
}

void SCServo::wheelSpeed( int value) {
  if (!wheelMode()) enableWheelMode();
  _wheelSpeed = value;
  sc.WritePWM(_id, value);
}

int SCServo::wheelSpeed() {
  return _wheelSpeed; 
}

void SCServo::torque(bool enabled) {
  sc.EnableTorque(_id, enabled);
  _torque = enabled;
}

bool SCServo::torque() {
  return _torque;
}

void SCServo::writePos(u16 pos, u16 time, u16 speed) {
  if (_wheelEnabled) return;
  sc.WritePos(_id, pos, time, speed);
}

void SCServo::changeId(u8 newId) {
  sc.unLockEprom(_id);
  sc.writeByte(_id, SCSCL_ID, newId);
  sc.LockEprom(newId);
  _id = newId;
}

bool SCServo::feedback() {
  return (sc.FeedBack(_id) > 0);
}

int SCServo::readPos(bool fromFeedback) {
  int id = (fromFeedback) ? -1 : _id;
  return sc.ReadPos(id);
}

int SCServo::readSpeed(bool fromFeedback) {
  int id = (fromFeedback) ? -1 : _id;
  return sc.ReadSpeed(id);
}

int SCServo::readLoad(bool fromFeedback) {
  int id = (fromFeedback) ? -1 : _id;
  return sc.ReadLoad(id);
}

int SCServo::readVoltage(bool fromFeedback) {
  int id = (fromFeedback) ? -1 : _id;
  return sc.ReadVoltage(id);
}

int SCServo::readTemper(bool fromFeedback) {
  int id = (fromFeedback) ? -1 : _id;
  return sc.ReadTemper(id);
}

int SCServo::readMove(bool fromFeedback) {
  int id = (fromFeedback) ? -1 : _id;
  return sc.ReadMove(id);
}

u8 SCServo::getId() {
  return _id;
}

void SCServo::stopServo() {
  sc.WritePos(_id, sc.ReadPos(_id), 0, 0);
  _angleGoal = -1;
}

void SCServo::update() {
  if (_angleGoal >= 0) {
    if (!readMove(false)) {
      if (callback) {
        String s = String("Ziel ") + String(getAngle(),2) + " erreicht.";
        callback(_id, CB_GOTO_ANGLE, 0, _angleGoal, s.c_str());
      }
      _angleGoal = -1;
    }
  }
}

void SCServo::setCallback(ServoCallback cb) {
  callback = cb;;
}

float SCServo::posToAngle(int pos) {
  return float(pos) * float(rotAngle) / float(rotValues-1) -5;
  // pos 20 -> 5 / 0
  // pos 1003 -> 293 / 

  // 0 * 300 / 1023 -> 0
  // 1023 * 300 / 1023
}

int SCServo::angleToPos(float angle) {
  return int((angle+5) * (rotValues-1) / rotAngle)+1;
  // Winkel 0 -> 20
  // Winkel 292 -> 1003
  // 0 * 1023 / 300 -> 0
  // 300 * 1023 / 300 -> 1023
}

void SCServo::gotoPos(int pos, int speed) {
  int fromPos = readPos(false);
  if (pos == fromPos) {
    // Serial.println("callback 1");
    // callback(_id, CB_GOTO_ANGLE, 0, angle, "Keine Bewegung notwendig");
    return;
  }
  if (speed == 0) {  // maximale Geschwindigkeit
    writePos(pos, 0, 0);
    while (!readMove(false)) delay(1);
    Serial.println("ohne callback, max speed");
    return;
  }
  writePos(pos, 0, speed);
}

void SCServo::gotoAngle(float angle, int speed) {
  gotoPos(angleToPos(angle), speed);
}
  
float SCServo::getAngle() {
  int pos = readPos(false);
  return posToAngle(pos);
};
  
int SCServo::getPos() {
  return readPos(false);
};

void SCServo::setMaxLoad(u16 max_load) {

}

