#pragma once
#include <Arduino.h>
#include <Bluepad32.h>
#include "arduino_multitasking.h"


extern ControllerPtr ps4Ctrl;

struct GamePad {
    uint8_t value;
    bool left, right, up, down;
};

struct Joystick {
    int16_t x, y;
    bool button;
};

struct Buttons {
    bool r1, l1, r2, l2;
    bool lStick, rStick;
};

struct Misc {
    bool ps, option, share;
};

struct Analog {
    int16_t l2, r2;
};

struct SensorData {
  int32_t gx, gy, gz;  // Gyro
  int32_t ax, ay, az;  // Accel
};

SensorData sensorData();
GamePad leftPad();
GamePad rightPad();
Joystick leftStick();
Joystick rightStick();
Buttons buttons();
Misc misc();
Analog analog();
static bool isPs4Controller(ControllerPtr gp);

class Ps4Controller : public Component {
  public:
    typedef void (*Ps4Callback)(Ps4Controller*);
    void setCallback(Ps4Callback cb);
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void rumble(uint16_t duration=500, uint8_t weak=120, uint8_t strong=200, uint16_t delayed=0);
    bool isConnected();
    String controllerName();

    void showPad(String name, GamePad& pad);
    void showStick(String name, Joystick& stick);
    void showButtons(Buttons& bt);
    void showMisc(Misc& bt);
    void showSensor(SensorData& s);
    void showAnalog(Analog& v);

    void loop();
    static void onConnectedGamepad(ControllerPtr gp);
    static void onDisconnectedGamepad(ControllerPtr gp);
    void setup();

  private:
    Ps4Callback callback = nullptr;
    bool wasConnected;
};



