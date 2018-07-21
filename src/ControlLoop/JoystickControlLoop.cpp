#include <Arduino.h>
//#include <NovaServo.h>
//#include <Joystick.h>
#include "JoystickControlLoop.h"

JoystickControlLoop::JoystickControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig) {
  _filterconst_left.x = novaConfig->_joy_left_config.x;
  _filterconst_left.y = novaConfig->_joy_left_config.y;
  _filterconst_right.x = novaConfig->_joy_right_config.x;
  _filterconst_right.y = novaConfig->_joy_right_config.y;

  _servo1 = hardwareConfig->servo1;
  _servo2 = hardwareConfig->servo2;
  _servo3 = hardwareConfig->servo3;
  _servo4 = hardwareConfig->servo4;
  _servo5 = hardwareConfig->servo5;

  _joy_left = hardwareConfig->joystick_left;
  _joy_right = hardwareConfig->joystick_right;
}

void JoystickControlLoop::observe() {
  _joy_left->getStatus(&_joy_left_input);
  _joy_right->getStatus(&_joy_right_input);
}

void JoystickControlLoop::actuate() {
  actuateContinousInputServo(_servo1,_joy_left_output.y);
  actuateContinousInputServo(_servo2,_joy_right_output.x);
  actuateContinousInputServo(_servo3,_joy_right_output.y);
  actuateContinousInputServo(_servo4,_joy_left_output.x);
  actuateStepwiseInputServo(_servo5);
}

void JoystickControlLoop::actuateContinousInputServo(NovaServo* servo, int degree) {
  if(degree > servo->getMinimum() && degree < servo->getMaximum())
    servo->setDegree(degree);
}

void JoystickControlLoop::actuateStepwiseInputServo(NovaServo* servo) {
  int servo_degree = servo->getDegree();
  if (_joy_left_input.sw) {
    if(servo_degree < servo->getMaximum()) servo_degree = servo_degree + _switch_degree_stepsize;
  }
  if (_joy_right_input.sw) {
    if(servo_degree > servo->getMinimum()) servo_degree = servo_degree - _switch_degree_stepsize;
  }
  servo->setDegree(servo_degree);
}

void JoystickControlLoop::mapInputToRange() {
  // servo4 has an inverted range, from high to low degrees;
  _joy_left_input.x = map(_joy_left_input.x, 0, 1024, _servo4->getMaximum(), _servo4->getMinimum());
  _joy_left_input.y = map(_joy_left_input.y, 0, 1024, _servo1->getMinimum(), _servo1->getMaximum());
  _joy_right_input.x = map(_joy_right_input.x, 0, 1024, _servo2->getMinimum(), _servo2->getMaximum());
  _joy_right_input.y = map(_joy_right_input.y, 0, 1024, _servo3->getMinimum(), _servo3->getMaximum());
}

void JoystickControlLoop::filterInput() {
  _joy_left_output.x = _joy_left_output.x * (1 - _filterconst_left.x) + _joy_left_input.x * _filterconst_left.x;
  _joy_left_output.y = _joy_left_output.y * (1 - _filterconst_left.y) + _joy_left_input.y * _filterconst_left.y;
  _joy_right_output.x = _joy_right_output.x * (1 - _filterconst_right.x) + _joy_right_input.x * _filterconst_right.x;
  _joy_right_output.y = _joy_right_output.y * (1 - _filterconst_right.y) + _joy_right_input.y * _filterconst_right.y;
}

void JoystickControlLoop::run() {
  observe();
  mapInputToRange();
  filterInput();
  actuate();
}
