#include <Arduino.h>
#include "KeyboardMouseControlLoop.h"

KeyboardMouseControlLoop::KeyboardMouseControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig) {
  _servo1 = hardwareConfig->servo1;
  _servo2 = hardwareConfig->servo2;
  _servo3 = hardwareConfig->servo3;
  _servo4 = hardwareConfig->servo4;
  _servo5 = hardwareConfig->servo5;
}

void KeyboardMouseControlLoop::observe(NovaCommand* cmd) {
  _offset_axis_1 = 0;
  _offset_axis_2 = 0;
  _offset_axis_3 = 0;
  _offset_axis_4 = 0;
  _offset_axis_5 = 0;

  switch (cmd->operandcode) {
    case NovaConstants::OP_KEYMOUSE_SET_AXIS_1:
      _offset_axis_1 = cmd->arg1;
      break;
    case NovaConstants::OP_KEYMOUSE_SET_AXIS_2:
      _offset_axis_2 = cmd->arg1;
      break;
    case NovaConstants::OP_KEYMOUSE_SET_AXIS_3:
      _offset_axis_3 = cmd->arg1;
      break;
    case NovaConstants::OP_KEYMOUSE_SET_AXIS_4:
      _offset_axis_4 = cmd->arg1;
      break;
    case NovaConstants::OP_KEYMOUSE_SET_AXIS_5:
      _offset_axis_5 = cmd->arg1;
      break;
  }
}

void KeyboardMouseControlLoop::actuate() {
  _servo1->setDegree(_servo1->getDegree() + _offset_axis_1);
  _servo2->setDegree(_servo2->getDegree() + _offset_axis_2);
  _servo3->setDegree(_servo3->getDegree() + _offset_axis_3);
  _servo4->setDegree(_servo4->getDegree() + _offset_axis_4);
  _servo5->setDegree(_servo5->getDegree() + _offset_axis_5);
}

void KeyboardMouseControlLoop::run(NovaCommand* cmd) {
  if(cmd != NULL && cmd->modulecode == NovaConstants::MOD_KEYBOARD_MOUSE_CONTROL) {
    observe(cmd);
    actuate();
  }
}
