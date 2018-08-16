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
  observe();
  computeControl();
  actuate();
}
