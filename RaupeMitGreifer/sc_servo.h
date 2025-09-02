#pragma once
#include "const.h"
#include <SCServo.h>

extern SCSCL sc;

class SCServo {
  public:
    SCServo(u8 servoId);

    // Basisfunktionen
    static bool ping(u8 id);
    
    void enableWheelMode();
    bool wheelMode();

    void wheelSpeed(int speed);
    int wheelSpeed();

    void torque(bool enabled); 
    bool torque(); //

    void stopServo();
    void writePos(u16 pos, u16 time, u16 speed); 

    bool feedback();  
    int readPos(bool fromFeedback=true); 
    int readSpeed(bool fromFeedback=true);
    int readLoad(bool fromFeedback=true); 
    int readVoltage(bool fromFeedback=true); 
    int readTemper(bool fromFeedback=true); 
    int readMove(bool fromFeedback=true); 
    
    u8 getId();
    void changeId(u8 newId);

    // erweiterte Funktionen
    void update();
    void setCallback(ServoCallback cb);
    float getAngle();
    int getPos();
    void setMaxLoad(u16 max_load);
    void gotoAngle(float angle, int speed=0);
    void gotoPos(int pos, int speed=0);

    // Hilfsfunktionen
    static float posToAngle(int pos);
    static int angleToPos(float angle);

    static const bool isST=false;
    static const int rotAngle=300;
    static const int rotValues=1024;  // ST: 4096
    static const int fullSpeed=1500;  // ST: 3073, steps per second
    static const int maxPWM = 1023;   // -maxPWM .. maxPWM im Motorbetrieb
    
    static const int minValue = 20;
    static const int maxValue = 1003;
    static const int minDeg = 5;
    static const int maxDeg = 293;

  private:
    static void init();

    ServoCallback callback;
    float _angleGoal;

    u8 _id;
    bool _wheelEnabled=false;
    int _wheelSpeed=0;
    bool _torque=false;

};





