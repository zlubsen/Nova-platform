#include <Arduino.h>
#include "JoystickRelativeControlLoop.h"

JoystickRelativeControlLoop::JoystickRelativeControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig) : JoystickAbsoluteControlLoop(hardwareConfig, novaConfig) {
  _max_degrees = novaConfig->_joystick_relative_degrees_range;

  _filterconst_left.x = novaConfig->_joy_left_config_relative.x;
  _filterconst_left.y = novaConfig->_joy_left_config_relative.y;
  _filterconst_right.x = novaConfig->_joy_right_config_relative.x;
  _filterconst_right.y = novaConfig->_joy_right_config_relative.y;

  // default output, to prevent excessive motion at startup
  _joy_left_output.x = 0;
  _joy_left_output.y = 0;
  _joy_right_output.x = 0;
  _joy_right_output.y = 0;
}

void JoystickRelativeControlLoop::mapInputToRange() {
  // servo4 has an inverted range, from high to low degrees;
  _joy_left_input.x = map(correctJoystickInput(_joy_left_input.x, 73), 0, 1023, _max_degrees, -_max_degrees);
  _joy_left_input.y = map(correctJoystickInput(_joy_left_input.y, 47), 0, 1023, -_max_degrees, _max_degrees);
  _joy_right_input.x = map(correctJoystickInput(_joy_right_input.x, 76), 0, 1023, -_max_degrees, _max_degrees);
  _joy_right_input.y = map(correctJoystickInput(_joy_right_input.y, 59), 0, 1023, -_max_degrees, _max_degrees);
}

void JoystickRelativeControlLoop::actuate() {
  _servo1->setDegree(_servo1->getDegree() + _joy_left_output.y);
  _servo2->setDegree(_servo2->getDegree() + _joy_right_output.x);
  _servo3->setDegree(_servo3->getDegree() + _joy_right_output.y);
  _servo4->setDegree(_servo4->getDegree() + _joy_left_output.x);
  actuateStepwiseInputServo(_servo5);
}

void JoystickRelativeControlLoop::run(NovaCommand* cmd) {
  if(_timer->elapsed()) {
    observe();
    mapInputToRange();
    filterInput();
    actuate();
  }
}

std::string JoystickRelativeControlLoop::getLCDStatusString() {
  std::string status(16, ' ');
  return status;
}
