#include <Arduino.h>
#include "JoystickAbsoluteControlLoop.h"

JoystickAbsoluteControlLoop::JoystickAbsoluteControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig) {
  _filterconst_left.x = novaConfig->_joy_left_config_absolute.x;
  _filterconst_left.y = novaConfig->_joy_left_config_absolute.y;
  _filterconst_right.x = novaConfig->_joy_right_config_absolute.x;
  _filterconst_right.y = novaConfig->_joy_right_config_absolute.y;

  _timer = new FrequencyTimer(novaConfig->_joystick_control_delay_frequency_ms);

  _servo1 = hardwareConfig->servo1;
  _servo2 = hardwareConfig->servo2;
  _servo3 = hardwareConfig->servo3;
  _servo4 = hardwareConfig->servo4;
  _servo5 = hardwareConfig->servo5;

  _joy_left = hardwareConfig->joystick_left;
  _joy_right = hardwareConfig->joystick_right;
}

void JoystickAbsoluteControlLoop::observe() {
  _joy_left->getStatus(&_joy_left_input);
  _joy_right->getStatus(&_joy_right_input);
}

void JoystickAbsoluteControlLoop::actuate() {
  _servo1->setDegree(_joy_left_output.y);
  _servo2->setDegree(_joy_right_output.x);
  _servo3->setDegree(_joy_right_output.y);
  _servo4->setDegree(_joy_left_output.x);
  actuateStepwiseInputServo(_servo5);
}

void JoystickAbsoluteControlLoop::actuateStepwiseInputServo(NovaServo* servo) {
  int servo_degree = servo->getDegree();
  if (_joy_left_input.sw) {
    if(servo_degree < servo->getMaximum()) servo_degree = servo_degree + _switch_degree_stepsize;
  }
  if (_joy_right_input.sw) {
    if(servo_degree > servo->getMinimum()) servo_degree = servo_degree - _switch_degree_stepsize;
  }
  servo->setDegree(servo_degree);
}

void JoystickAbsoluteControlLoop::mapInputToRange() {
  // servo4 has an inverted range, from high to low degrees;
  _joy_left_input.x = map(_joy_left_input.x, 0, 1024, _servo4->getMaximum(), _servo4->getMinimum());
  _joy_left_input.y = map(_joy_left_input.y, 0, 1024, _servo1->getMinimum(), _servo1->getMaximum());
  _joy_right_input.x = map(_joy_right_input.x, 0, 1024, _servo2->getMinimum(), _servo2->getMaximum());
  _joy_right_input.y = map(_joy_right_input.y, 0, 1024, _servo3->getMinimum(), _servo3->getMaximum());
}

void JoystickAbsoluteControlLoop::filterInput() {
  _joy_left_output.x = _joy_left_output.x * (1 - _filterconst_left.x) + _joy_left_input.x * _filterconst_left.x;
  _joy_left_output.y = _joy_left_output.y * (1 - _filterconst_left.y) + _joy_left_input.y * _filterconst_left.y;
  _joy_right_output.x = _joy_right_output.x * (1 - _filterconst_right.x) + _joy_right_input.x * _filterconst_right.x;
  _joy_right_output.y = _joy_right_output.y * (1 - _filterconst_right.y) + _joy_right_input.y * _filterconst_right.y;
}

void JoystickAbsoluteControlLoop::run(NovaCommand* cmd) {
  if(_timer->elapsed()) {
    observe();
    mapInputToRange();
    filterInput();
    actuate();
  }
}