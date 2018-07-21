#include <Arduino.h>
#include <HardwareConfig.h>

HardwareConfig::HardwareConfig() {
  configureServos();
  configureJoysticks();
  configureUltrasound();
}

void HardwareConfig::configureServos() {
  servo1 = new NovaServo(32, 85, 160); // mid: 122.5
  servo2 = new NovaServo(34, 0, 180); // mid: 90
  servo3 = new NovaServo(36, 35, 175); // mid: 105
  servo4 = new NovaServo(38, 10, 170); // mid: 90
  servo5 = new NovaServo(40, 75, 145); // mid: 110
}

void HardwareConfig::configureJoysticks() {
  joystick_left = new Joystick(A9, A11, 43);
  joystick_right = new Joystick(A13, A15, 45);
}

void HardwareConfig::configureUltrasound() {
  // TODO
}
