# Einsatz von BluePad32 für die Fernsteuerung eines Modells
Das ist kein vollständiges Projekt. Es dient lediglich dem Test der Fernbedienung und der seriellen Busservos.  
Die Verwendung des Codes läuft auf eigene Gefahr. Ausserden sind die Lizenzbedingungen der eingesetzen Bibliotheken zu beachten!

Es wird ein Raupenfahrzeug motorisiert, das mit einem Greifer ausgestattet ist.   
Zur Fernsteuerung wird ein PS4 - Controller eingesetzt. Dieser kommuniziert per Bluetooth mit dem ESP32 Board.   
Als Motoren und Servos werden serielle Busservos (SC09) von Waveshare verwendet.   

![Raupenfahrzeug](https://github.com/user-attachments/assets/9ed05d4d-2fee-4f67-9e1a-7a87be72b6fa)


# Arduino IDE einrichten
Es wird die Arduino IDE 2.x verwendet.
Zur Fernsteuerung wird ein PS4 Controller mit Hilfe von BluePad32 mit dem ESP32 Board verbunden. BluePad32 steht aber nicht als Library zur Verfügung. Stattdessen wird es als Board installiert. 

https://bluepad32.readthedocs.io/en/latest/plat_arduino/

## Boardmanager URLs
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
https://raw.githubusercontent.com/ricardoquesada/esp32-arduino-lib-builder/master/bluepad32_files/package_esp32_bluepad32_index.json

## Einstellungen Board
Als Board wird ESP32 Dev Module - esp32_bluepad32 ausgewählt.  
Partition Scheme: Huge APP (3MB No OTA/1MB SPIFFS)  
Upload Speed: 921600

## Libraries
FTServo by ftservo  
Adafruit Neopixel  

# Hardware
PS4 Controller  
https://de.aliexpress.com/item/1005007144726230.html

ESP32 Board  
https://www.bastelgarage.ch/esp32-serial-bus-servo-driver-expansion-board?search=422292
https://www.waveshare.com/servo-driver-with-esp32.htm
https://www.waveshare.com/wiki/Servo_Driver_with_ESP32

5 x SC09 Servo  
https://www.waveshare.com/wiki/SC09_Servo
https://www.bastelgarage.ch/sc09-2-3kg-serial-bus-servo?search=sc09

# Eigene Bibliotheksdateien
arduino_multitasking .h / .cpp&nbsp;&nbsp;&nbsp;(Multitasking Unterstützung)  
ps4_controller .h / .cpp&nbsp;&nbsp;&nbsp;(PS4 Controller Unterstützung)  
sc_servo .h / .cpp&nbsp;&nbsp;&nbsp;(Unterstützung für Servos der SC - Serie)  
servos .h / .cpp&nbsp;&nbsp;&nbsp;(ServoSC und MotorSC)  

Diese Dateien sind im Prinzip nicht projektspezifisch. Sie sind aber nicht vollständig und unterstützen teilweise nur die Funktionen, die das Projekt aktuell verwendet. 

# Projektdateien
RaupeMitGreifer.ino&nbsp;&nbsp;&nbsp;(Hauptdatei)  
model .h / .cpp&nbsp;&nbsp;&nbsp;(Komponenten des Modells und ihre Interaktionen)  
const.h&nbsp;&nbsp;&nbsp;          (Global verwendete Konstanten und Typendeklarationen)  
RGB_CRL.h / .cpp&nbsp;&nbsp;&nbsp;                          (Spielereien mit den RGB Leds)  

Diese Dateien sind projektspezifisch.


