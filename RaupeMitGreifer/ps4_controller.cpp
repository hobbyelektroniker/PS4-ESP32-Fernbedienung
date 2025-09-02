#include "ps4_controller.h"

ControllerPtr ps4Ctrl = nullptr;

SensorData sensorData() {
  SensorData data;
  data.gx = ps4Ctrl->gyroX();
  data.gy = ps4Ctrl->gyroY();
  data.gz = ps4Ctrl->gyroZ();
  data.ax = ps4Ctrl->accelX();
  data.ay = ps4Ctrl->accelY();
  data.az = ps4Ctrl->accelZ();
  return data;  // Kopie wird zurückgegeben
}

GamePad leftPad() {
    GamePad data;
    data.value = ps4Ctrl->dpad();
    data.left = (data.value & 8);
    data.down = (data.value & 2);
    data.right = (data.value & 4);
    data.up = (data.value & 1);
    return data;
}

GamePad rightPad() {
    GamePad data;
    data.left = ps4Ctrl->x();
    data.right= ps4Ctrl->b();
    data.up = ps4Ctrl->y();
    data.down = ps4Ctrl->a();
    data.value = data.left + data.right + data.up + data.down;
    return data;
}

Joystick leftStick() {
  Joystick data;
  data.x = ps4Ctrl->axisX();  // positiv: rechts / unten
  data.y = ps4Ctrl->axisY();  // positiv: rechts / unten
  uint16_t button = ps4Ctrl->buttons();
  data.button = (button & 256);
  return data;
}

Joystick rightStick() {
  Joystick data;
  data.x = ps4Ctrl->axisRX();  // positiv: rechts / unten
  data.y = ps4Ctrl->axisRY();  // positiv: rechts / unten
  uint16_t button = ps4Ctrl->buttons();
  data.button = (button & 512);
  return data;
}

Buttons buttons() {
  Buttons data;
  uint16_t bt = ps4Ctrl->buttons();
  data.l1 = (bt & 16);
  data.l2 = (bt & 64);
  data.r1 = (bt & 32);
  data.r2 = (bt & 128);
  data.lStick = (bt & 256);
  data.rStick = (bt & 512);
  return data;
}

Misc misc() {
  Misc data;
  uint16_t bt = ps4Ctrl->miscButtons();
  data.ps = (bt & 1);
  data.share = (bt & 2);
  data.option = (bt & 4);
  return data;
}

Analog analog() {
  Analog data;
  data.l2 = ps4Ctrl->brake();
  data.r2 = ps4Ctrl->throttle();
  return data;
}

static bool isPs4Controller(ControllerPtr gp) {
  // Robust: per Modellname prüfen (funktioniert in allen Versionen)
  String nameString = gp->getModelName();
  const char* name = nameString.c_str();
  if (!name) return false;
  // Häufige Namen: "Wireless Controller", "DualShock 4", teils "PS4 Controller"
  return strstr(name, "Wireless Controller") ||
         strstr(name, "DualShock") ||
         strstr(name, "PS4");
  // Wenn deine Bluepad32-Version ein Modell-Enum hat:
  // return gp->getProperties().model == GamepadModel::PS4;
}

void Ps4Controller::setCallback(Ps4Callback cb) {
    callback = cb;
}

void Ps4Controller::setColor(uint8_t r, uint8_t g, uint8_t b) {
  ps4Ctrl->setColorLED(r, g, b);
}

void Ps4Controller::rumble(uint16_t duration, uint8_t weak, uint8_t strong, uint16_t delayed) {
  ps4Ctrl->playDualRumble(delayed, duration, weak, strong);
}

bool Ps4Controller::isConnected() {
    return (ps4Ctrl && ps4Ctrl->isConnected());
}

String Ps4Controller::controllerName() {
    if (isConnected()) {
        return ps4Ctrl->getModelName();
    } else {
        return String("not connected");
    }
}

void Ps4Controller::showPad(String name, GamePad& pad) {
    Serial.printf("name=%s, value=%4d  left=%d  right=%d, up=%d, down=%d\n", name, pad.value, pad.left, pad.right, pad.up, pad.down);
}

void Ps4Controller::showStick(String name, Joystick& stick) {
    Serial.printf("name=%s, x=%4d  y=%d  botton=%d\n", name, stick.x, stick.y, stick.button);
}

void Ps4Controller::showButtons(Buttons& bt) {
    Serial.printf("L1=%d, R1=%d,  L2=%d, R2=%d, lStick=%d, rStick=%d\n", bt.l1, bt.r1, bt.l2, bt.r2, bt.lStick, bt.rStick);
}

void Ps4Controller::showMisc(Misc& bt) {
    Serial.printf("PS=%d, share=%d,  option=%d\n", bt.ps, bt.share, bt.option);
}

void Ps4Controller::showSensor(SensorData& s) {
    Serial.printf("gyroX=%6d, gyroY=%6d  gyroZ=%6d, accelX=%6d, accelY=%6d, accelZ=%6d\n", 
                    s.gx, s.gy, s.gz, s.ax, s.ay, s.az);
}

void Ps4Controller::showAnalog(Analog& v) {
    Serial.printf("l2 (brake)=%4d, r2 (throttle)=%4d\n", v.l2, v.r2);
}

void Ps4Controller::loop() {
    BP32.update();
    if (isConnected()) {
      wasConnected = true;
      if (callback) callback(this);
    } else  {
      if (wasConnected) {
        Serial.println("Warten auf Verbindung...");
        wait(100);
      }
      wasConnected = false;
    }
}

void Ps4Controller::onConnectedGamepad(ControllerPtr gp) {
  if (!isPs4Controller(gp)) {
    Serial.printf("Nicht-PS4 erkannt (%s) → trenne.\n", gp->getModelName());
    gp->disconnect();
    return;
  }

  if (!ps4Ctrl) {

    ps4Ctrl = gp;
    Serial.printf("PS4-Controller verbunden: idx=%d, %s\n", gp->index(), gp->getModelName());
    // Weitere Verbindungen unterbinden:
    BP32.enableNewBluetoothConnections(false);
  } else {
    // Es gibt schon einen PS4-Controller → zusätzlichen sofort trennen
    Serial.println("Es ist bereits ein PS4-Controller verbunden → trenne zusätzlichen.");
    gp->disconnect();
  }
}

void Ps4Controller::onDisconnectedGamepad(ControllerPtr gp) {
  if (ps4Ctrl == gp) {
    Serial.println("PS4-Controller getrennt.");
    ps4Ctrl = nullptr;
    // Wieder neue Verbindungen erlauben:
    BP32.enableNewBluetoothConnections(true);
    // Ps4Controller* ctl = (Ps4Controller*)gp;
    // ctl->waitForConnect();
  }
}

void Ps4Controller::setup() {
    Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
    const uint8_t* addr = BP32.localBdAddress();
    Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
    wasConnected = true;
    BP32.setup(&Ps4Controller::onConnectedGamepad, &Ps4Controller::onDisconnectedGamepad);
    // BP32.forgetBluetoothKeys();
    delay(2000);
}





