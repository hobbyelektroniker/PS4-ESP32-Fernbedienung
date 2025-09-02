
// the uart used to control servos.
// GPIO 18 - S_RXD, GPIO 19 - S_TXD, as default.

#define S_RXD 18
#define S_TXD 19

typedef void (*ServoCallback)(int servoIdId, int type, int i, float f, const char* info);

#define CB_GOTO_ANGLE 1
#define CB_MAX_LOAD 2

