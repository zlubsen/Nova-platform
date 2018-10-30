#include "StatusPublishLoop.h"

StatusPublishLoop::StatusPublishLoop(HardwareConfig* hardwareConfig, int frequency_ms) {
  _comm = hardwareConfig->comm;
  _timer = new FrequencyTimer(frequency_ms);

  servo1 = hardwareConfig->servo1;
  servo2 = hardwareConfig->servo2;
  servo3 = hardwareConfig->servo3;
  servo4 = hardwareConfig->servo4;
  servo5 = hardwareConfig->servo5;
  ultraSoundSensor = hardwareConfig->ultraSoundSensor;

}

void StatusPublishLoop::run(NovaProtocolCommand* cmd) {
  if(_timer->elapsed()) {
    _comm->writeCommand(cmd_nova, cmd_module, cmd_get_degree, servo1->getDegree());
    _comm->writeCommand(cmd_nova, cmd_module, cmd_get_degree, servo2->getDegree());
    _comm->writeCommand(cmd_nova, cmd_module, cmd_get_degree, servo3->getDegree());
    _comm->writeCommand(cmd_nova, cmd_module, cmd_get_degree, servo4->getDegree());
    _comm->writeCommand(cmd_nova, cmd_module, cmd_get_degree, servo5->getDegree());
    _comm->writeCommand(cmd_nova, cmd_module, cmd_get_distance, ultraSoundSensor->measureDistance());
  }
}

std::string StatusPublishLoop::getLCDStatusString() {
  std::string status(16, ' ');
  return status;
}
