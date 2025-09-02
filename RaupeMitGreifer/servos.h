#pragma once
#include "arduino_multitasking.h"
#include "sc_servo.h"


class MotorSC : public Component {
  public:
 
    void block();
    void release();
    void stop();

    bool isAccelerating();
    void targetReached();
    void setAccelTime(long accelTime);
    void setSpeedTarget(int value, long accelTime=-1);
    void speed(int value);
    int speed();
    
    void loop();
    void setup(u8 id);

    const int maxPWM = SCServo::maxPWM;

  protected:
    long _accelTime;
    
    int _targetSpeed;
    int _speed;
    int _speedStep;
    u16 _steps;
    u8 _id;
    SCServo* servo = nullptr;
};

class ServoSC : public Component {
  public:
 
    void block();
    void release();
    void stop();

    void targetReached();
    void gotoAngle(float angle, int speed=0);
    void gotoPos(int pos, int speed=0);
    
    void loop();
    void setup(u8 id);

    static const int maxPWM = SCServo::maxPWM;
    static const int rotAngle = SCServo::rotAngle;
    static const int rotValues = SCServo::rotValues;
    static const int fullSpeed = SCServo::fullSpeed;  // steps per second
    static const int minDeg = SCServo::minDeg;
    static const int maxDeg = SCServo::maxDeg; 

    SCServo* servo = nullptr;


  protected:
    u8 _id;

};

