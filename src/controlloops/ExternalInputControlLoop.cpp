#include <Arduino.h>
#include "ExternalInputControlLoop.h"

ExternalInputControlLoop::ExternalInputControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig) {
  _servo1 = hardwareConfig->servo1;
  _servo2 = hardwareConfig->servo2;
  _servo3 = hardwareConfig->servo3;
  _servo4 = hardwareConfig->servo4;
  _servo5 = hardwareConfig->servo5;
}

void ExternalInputControlLoop::observe(NovaProtocolCommand* cmd) {
  if(!cmd->args.empty()) {
    _offset_axis_1 = 0;
    _offset_axis_2 = 0;
    _offset_axis_3 = 0;
    _offset_axis_4 = 0;
    _offset_axis_5 = 0;

    int degree_value = cmd->args.at(0);

    switch (cmd->asset) {
      case cmd_servo1:
        _offset_axis_1 = degree_value;
        break;
      case cmd_servo2:
        _offset_axis_2 = degree_value;
        break;
      case cmd_servo3:
        _offset_axis_3 = degree_value;
        break;
      case cmd_servo4:
        _offset_axis_4 = degree_value;
        break;
      case cmd_servo5:
        _offset_axis_5 = degree_value;
        break;
    }
  }
}

void ExternalInputControlLoop::actuate() {
  _servo1->setDegree(_servo1->getDegree() + _offset_axis_1);
  _servo2->setDegree(_servo2->getDegree() + _offset_axis_2);
  _servo3->setDegree(_servo3->getDegree() + _offset_axis_3);
  _servo4->setDegree(_servo4->getDegree() + _offset_axis_4);
  _servo5->setDegree(_servo5->getDegree() + _offset_axis_5);
}

void ExternalInputControlLoop::run(NovaProtocolCommand* cmd) {
  if(cmd != nullptr && cmd->module == cmd_external_input) {
    observe(cmd);
    actuate();
  }
}

std::string ExternalInputControlLoop::getLCDStatusString() {
  std::string status(16, ' ');
  return status;
}
