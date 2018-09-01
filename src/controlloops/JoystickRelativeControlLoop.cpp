#include <Arduino.h>
#include "JoystickRelativeControlLoop.h"

JoystickRelativeControlLoop::JoystickRelativeControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig) : JoystickControlLoop(hardwareConfig, novaConfig) {
  _max_degrees = novaConfig->_joystick_relative_degrees_range;
}

void JoystickRelativeControlLoop::mapInputToRange() {
  // servo4 has an inverted range, from high to low degrees;
  _joy_left_input.x = map(_joy_left_input.x, 0, 1024, _max_degrees, -_max_degrees);
  _joy_left_input.y = map(_joy_left_input.y, 0, 1024, -_max_degrees, _max_degrees);
  _joy_right_input.x = map(_joy_right_input.x, 0, 1024, -_max_degrees, _max_degrees);
  _joy_right_input.y = map(_joy_right_input.y, 0, 1024, -_max_degrees, _max_degrees);
}

void JoystickRelativeControlLoop::actuate() {
  _servo1->setDegree(_servo1->getDegree() + _joy_left_output.y);
  _servo2->setDegree(_servo2->getDegree() + _joy_right_output.x);
  _servo3->setDegree(_servo3->getDegree() + _joy_right_output.y);
  _servo4->setDegree(_servo4->getDegree() + _joy_left_output.x);
  actuateStepwiseInputServo(_servo5);
}
