#pragma once
#include "arduino_multitasking.h"
#include "RGB_CTRL.h"
#include "ps4_controller.h"
#include "servos.h"

class Hub : public Component {
  public:

    void up();
    void down();
    void stop();
    void loop();
    void setup(u8 id);

    ServoSC motor;

  private:
    enum class States {
      UNDEFINED, STOP, UP, DOWN 
    };
    
    States state;
    int oldPos=-1;
};

class GreiferDrehung : public Component {
  public:

    void left();
    void right();
    void stop();
    void loop();
    void setup(u8 id);

    ServoSC motor;

  private:
    enum class States {
      UNDEFINED, STOP, LEFT, RIGHT 
    };
   
    States state;
    int oldPos=-1;
};

class Greifer : public Component {
  public:

    void open();
    void close();
    void stop();
    void loop();
    void setup(u8 id);

    ServoSC motor;

  private:
    enum class States {
      UNDEFINED, STOP, OPEN, CLOSE 
    };
    
    States state;
    int oldPos=-1;
};

extern RgbMatrix leds;
extern Ps4Controller controller;
extern Hub hub;
extern MotorSC leftMotor;
extern MotorSC rightMotor;
extern GreiferDrehung greiferDrehung;
extern Greifer greifer;

void onRemote(Ps4Controller* c);
void initModel();

