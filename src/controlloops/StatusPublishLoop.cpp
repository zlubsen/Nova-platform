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

void StatusPublishLoop::run(NovaCommand* cmd) {
  if(_timer->elapsed()) {
    _comm->writeCommand(NovaConstants::MOD_STATUS_NOVA, NovaConstants::OP_STATUS_RECEIVE_SERVO_1, servo1->getDegree(),0,0);
    _comm->writeCommand(NovaConstants::MOD_STATUS_NOVA, NovaConstants::OP_STATUS_RECEIVE_SERVO_2, servo2->getDegree(),0,0);
    _comm->writeCommand(NovaConstants::MOD_STATUS_NOVA, NovaConstants::OP_STATUS_RECEIVE_SERVO_3, servo3->getDegree(),0,0);
    _comm->writeCommand(NovaConstants::MOD_STATUS_NOVA, NovaConstants::OP_STATUS_RECEIVE_SERVO_4, servo4->getDegree(),0,0);
    _comm->writeCommand(NovaConstants::MOD_STATUS_NOVA, NovaConstants::OP_STATUS_RECEIVE_SERVO_5, servo5->getDegree(),0,0);
    _comm->writeCommand(NovaConstants::MOD_STATUS_NOVA, NovaConstants::OP_STATUS_RECEIVE_USS, ultraSoundSensor->measureDistance(),0,0);
  }
}
