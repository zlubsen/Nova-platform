#ifndef Hardware_Config_h
#define Hardware_Config_h

#include <actuators/NovaServo.h>
#include <sensors/Joystick.h>
#include <sensors/UltraSoundSensor.h>
#include <communication/SerialCommunication.h>

typedef struct {
  int pin;
  int min_range;
  int max_range;
} servo_config;

typedef struct {
  int x_pin;
  int y_pin;
  int switch_pin;
} joystick_config;

class HardwareConfig {
  public:
    HardwareConfig();
    void configureServos();
    void configureJoysticks();
    void configureUltrasound();
    void configureCommunication();
    void resetServosToMiddle();
    NovaServo* servo1;
    NovaServo* servo2;
    NovaServo* servo3;
    NovaServo* servo4;
    NovaServo* servo5;
    Joystick* joystick_left;
    Joystick* joystick_right;
    UltraSoundSensor* ultraSoundSensor;
    SerialCommunication* comm;
};

#endif
