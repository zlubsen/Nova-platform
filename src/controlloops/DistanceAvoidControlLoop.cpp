#include <Arduino.h>
#include "DistanceAvoidControlLoop.h"

DistanceAvoidControlLoop::DistanceAvoidControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig) {
  _comm = hardwareConfig->comm;
  _ultraSoundSensor = hardwareConfig->ultraSoundSensor;
  _servo = hardwareConfig->servo1;
  _pid_config = novaConfig->_distance_avoid_pid_config;

  _pid_values.setpoint = novaConfig->_ultrasound_config.setpoint;
  _min_distance = novaConfig->_ultrasound_config.minDistance;
  _max_distance = novaConfig->_ultrasound_config.maxDistance;
  _servo_angle = novaConfig->_ultrasound_config.servoAngle;

  setupPIDcontroller();
  statusPublishPIDValues();
}

void DistanceAvoidControlLoop::setupPIDcontroller() {
  _pid = new PID(&_pid_values.input,
    &_pid_values.output,
    &_pid_values.setpoint,
    _pid_config.Kp,
    _pid_config.Ki,
    _pid_config.Kd,
    _pid_config.direction);

    _pid->SetMode(_pid_config.mode);
    _pid->SetSampleTime(_pid_config.sampleTime);
    _pid->SetOutputLimits(_pid_config.outputLimitMin, _pid_config.outputLimitMax);
}

void DistanceAvoidControlLoop::handleCommands(NovaCommand* cmd) {
  switch (cmd->operandcode) {
    case NovaConstants::OP_DISTANCE_SET_MIN_DIST:
      setMinimumDistanceLimit(cmd->arg1);
      break;
    case NovaConstants::OP_DISTANCE_SET_MAX_DIST:
      setMaximumDistanceLimit(cmd->arg1);
      break;
    case NovaConstants::OP_DISTANCE_SET_SETPOINT:
      setSetpoint(cmd->arg1);
      break;
    case NovaConstants::OP_DISTANCE_SET_PID_TUNING:
      setPIDTuning(cmd->arg1, cmd->arg2, cmd->arg3);
      break;
  }
}

void DistanceAvoidControlLoop::setMinimumDistanceLimit(int new_distance) {
  if(new_distance < _max_distance)
    _min_distance = new_distance;
}

void DistanceAvoidControlLoop::setMaximumDistanceLimit(int new_distance) {
  if(new_distance > _min_distance)
    _max_distance = new_distance;
}

void DistanceAvoidControlLoop::setSetpoint(int new_distance) {
  if(new_distance >= _min_distance && new_distance <= _max_distance)
    _pid_values.setpoint = new_distance;
}

void DistanceAvoidControlLoop::setPIDTuning(int p_value, int i_value, int d_value) {
  double new_p = ((double) p_value)/1000;
  double new_i = ((double) i_value)/1000;
  double new_d = ((double) d_value)/1000;

  _pid->SetTunings(new_p, new_i, new_d);

  statusPublishPIDValues();
}

void DistanceAvoidControlLoop::statusPublishPIDValues() {
  int Kp = (int)(_pid->GetKp()*1000);
  int Ki = (int)(_pid->GetKi()*1000);
  int Kd = (int)(_pid->GetKd()*1000);
  _comm->writeCommand(NovaConstants::MOD_STATUS_NOVA, NovaConstants::OP_STATUS_RECEIVE_DISTANCE_PID, Kp, Ki, Kd);
}

void DistanceAvoidControlLoop::observe() {
  _pid_values.input = _ultraSoundSensor->measureDistance();
}

void DistanceAvoidControlLoop::actuate() {
  if(_pid_values.input < _max_distance && _pid_values.input > _min_distance) {
       _servo_angle = _servo->getDegree() - _pid_values.output;

       _servo->setDegree(_servo_angle);
  }
}

void DistanceAvoidControlLoop::computeControl() {
  _pid->Compute();
}

void DistanceAvoidControlLoop::run(NovaCommand* cmd) {
  if(cmd != NULL && cmd->modulecode == NovaConstants::MOD_DISTANCE_AVOIDANCE) {
    handleCommands(cmd);
  }
  observe();
  computeControl();
  actuate();

  //_comm->writeCommand(99,0,_pid_values.input,_pid_values.output,_servo_angle);
}

String DistanceAvoidControlLoop::getLCDStatusString() {
  String str_start = "Dist:";
  String str_value = String((int)_pid_values.input);
  String str_end = " cm";
  int text_length = str_start.length() + str_value.length() + str_end.length();
  String mid_padding = "";

  for(int i = (16-text_length); i > 0; i--) {
    mid_padding.concat(" ");
  }

  return (str_start + mid_padding + str_value + str_end);
}
