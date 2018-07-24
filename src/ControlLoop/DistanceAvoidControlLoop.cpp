#include <Arduino.h>
#include "DistanceAvoidControlLoop.h"

DistanceAvoidControlLoop::DistanceAvoidControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig) {
  _ultraSoundSensor = hardwareConfig->ultraSoundSensor;
  _servo = hardwareConfig->servo1;
  _pid_config = novaConfig->_ultrasound_pid_config;

  _setpoint_distance = novaConfig->_ultrasound_config.setpoint;
  _min_distance = novaConfig->_ultrasound_config.minDistance;
  _max_distance = novaConfig->_ultrasound_config.maxDistance;
  _servo_angle = novaConfig->_ultrasound_config.servoAngle;

  setupPIDcontroller();
}

void DistanceAvoidControlLoop::setupPIDcontroller() {
  _pid = new PID(&_input_distance,
    &_output_distance,
    &_setpoint_distance,
    _pid_config.Kp,
    _pid_config.Ki,
    _pid_config.Kd,
    DIRECT);

    _pid->SetMode(AUTOMATIC);
    _pid->SetSampleTime(_pid_config.sampleTime);
    _pid->SetOutputLimits(_pid_config.outputLimitMin, _pid_config.outputLimitMax);
}

void DistanceAvoidControlLoop::observe() {
  _input_distance = _ultraSoundSensor->measureDistance();
}

void DistanceAvoidControlLoop::actuate() {
  if(_input_distance < _max_distance && _input_distance > _min_distance) {
       _servo_angle = _servo_angle - _output_distance;

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

void DistanceAvoidControlLoop::run() {
  observe();
  computeControl();
  actuate();
}
