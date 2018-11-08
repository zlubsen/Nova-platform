#include "StatusPublishLoop.hpp"
#include <vector>
#include <MemoryFree.hpp>

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
    //writeStatusCommand(cmd_get_degree, servo1->getDegree());
    //writeStatusCommand(cmd_get_degree, servo2->getDegree());
    //writeStatusCommand(cmd_get_degree, servo3->getDegree());
    //writeStatusCommand(cmd_get_degree, servo4->getDegree());
    //writeStatusCommand(cmd_get_degree, servo5->getDegree());
    //writeStatusCommand(cmd_get_distance, ultraSoundSensor->measureDistance());
    //writeStatusCommand(cmd_get_freememory, freeMemory());
  }
}

void StatusPublishLoop::writeStatusCommand(uint8_t operation, int arg) {
  NovaProtocolCommandBuilder* builder = _comm->getBuilder();

  std::vector<int> args {arg};

  _comm->writeCommand(builder
    ->setModule(cmd_nova)
    ->setAsset(cmd_module)
    ->setOperation(operation)
    ->setArgs(args)
    ->build());
}

std::string StatusPublishLoop::getLCDStatusString() {
  std::string status(16, ' ');
  return status;
}
