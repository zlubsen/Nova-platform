#include <Arduino.h>
#include "DistanceAvoidControlLoop.h"

DistanceAvoidControlLoop::DistanceAvoidControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig) {
  _ultraSoundSensor = hardwareConfig->ultraSoundSensor;
  _servo = hardwareConfig->servo1;
  _pid_config = novaConfig->_distance_avoid_pid_config;

  _pid_values.setpoint = novaConfig->_ultrasound_config.setpoint;
  _min_distance = novaConfig->_ultrasound_config.minDistance;
  _max_distance = novaConfig->_ultrasound_config.maxDistance;
  _servo_angle = novaConfig->_ultrasound_config.servoAngle;

  setupPIDcontroller();
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
      setDistanceLimit(true, cmd->arg1);
      break;
    case NovaConstants::OP_DISTANCE_SET_MAX_DIST:
      setDistanceLimit(false, cmd->arg1);
      break;
    case NovaConstants::OP_DISTANCE_SET_SETPOINT:
      setSetpoint(cmd->arg1);
      break;
    case NovaConstants::OP_DISTANCE_SET_PID_TUNING:
      setPIDTuning(cmd->arg1, cmd->arg2, cmd->arg3);
      break;
  }
}

void DistanceAvoidControlLoop::setDistanceLimit(bool set_minimum, int new_distance) {
  // TODO validate that new_distance is a valid min or max distance
  if(set_minimum) {
    _min_distance = new_distance;
  } else {
    _max_distance = new_distance;
  }
}

void DistanceAvoidControlLoop::setSetpoint(int new_distance) {
  // TODO validate setpoint is in a valid range... (between _min_distance and _max_distance)
  _pid_values.setpoint = new_distance;
}

void DistanceAvoidControlLoop::setPIDTuning(int p_value, int i_value, int d_value) {
  double new_p = ((double) p_value)/1000;
  double new_i = ((double) i_value)/1000;
  double new_d = ((double) d_value)/1000;

  _pid->SetTunings(new_p, new_i, new_d);
}

void DistanceAvoidControlLoop::observe() {
  _pid_values.input = _ultraSoundSensor->measureDistance();
}

void DistanceAvoidControlLoop::actuate() {
  if(_pid_values.input < _max_distance && _pid_values.input > _min_distance) {
       _servo_angle = _servo_angle - _pid_values.output;

       if(_servo_angle > _servo->getMaximum())
          _servo_angle = _servo->getMaximum();
       if(_servo_angle < _servo->getMinimum())
          _servo_angle = _servo->getMinimum();

       _servo->setDegree(_servo_angle);
  }
}

void DistanceAvoidControlLoop::computeControl() {
  _pid->Compute();
}

void DistanceAvoidControlLoop::run(NovaCommand* cmd) {
  if(cmd != NULL && cmd->modulecode == NovaConstants::MOD_DISTANCE_AVOIDANCE) {
    handleCommands(cmd);
    delete cmd;
  }
  observe();
  computeControl();
  actuate();
}
