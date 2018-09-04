#include <Arduino.h>
#include "HardwareConfig.h"

HardwareConfig::HardwareConfig() {
  configureServos();
  configureJoysticks();
  configureUltrasound();
  configureCommunication();
}

void HardwareConfig::configureServos() {
  servo1 = new NovaServo(32, 85, 160); // mid: 122.5  // Head Movement - Front and Back
  servo2 = new NovaServo(34, 5, 175); // mid: 90      // Head Rotation - Clockwise and Anticlockwise
  servo3 = new NovaServo(36, 35, 175); // mid: 105    // Head Rotation - Up and Down
  servo4 = new NovaServo(38, 10, 170); // mid: 90     // Whole Body Rotation - Z axis
  servo5 = new NovaServo(40, 75, 145); // mid: 110    // Head Movement - Up and Down
}

void HardwareConfig::configureJoysticks() {
  joystick_left = new Joystick(A9, A11, 43);
  joystick_right = new Joystick(A13, A15, 45);
}

void HardwareConfig::configureUltrasound() {
  ultraSoundSensor = new UltraSoundSensor(46, 44);
}

void HardwareConfig::configureCommunication() {
  comm = new SerialCommunication(28800);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
}
