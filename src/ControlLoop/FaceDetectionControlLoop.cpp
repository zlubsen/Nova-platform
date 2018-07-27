#include <Arduino.h>
#include "FaceDetectionControlLoop.h"

FaceDetectionControlLoop::FaceDetectionControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig) {
  _servo_x = hardwareConfig->servo4;
  _servo_y = hardwareConfig->servo3;

  _pid_config_x = novaConfig->_face_detection_pid_config_x;
  _pid_values_x.setpoint = 90; // TODO move to NovaConfig, or get from servo (middle)
  setupPIDcontroller(_pid_x, &_pid_config_x, &_pid_values_x);

  _pid_config_y = novaConfig->_face_detection_pid_config_y;
  _pid_values_y.setpoint = 90; // TODO move to NovaConfig, or get from servo (middle)
  setupPIDcontroller(_pid_y, &_pid_config_y, &_pid_values_y);
}

void FaceDetectionControlLoop::setupPIDcontroller(PID* pid, pid_config* config, pid_dynamic_values* values) {
  pid = new PID(&values->input,
    &values->output,
    &values->setpoint,
    config->Kp,
    config->Ki,
    config->Kd,
    config->direction);

    pid->SetMode(config->mode);
    pid->SetSampleTime(config->sampleTime);
    pid->SetOutputLimits(config->outputLimitMin, config->outputLimitMax);
}

void FaceDetectionControlLoop::observe() {
  _pid_values_x.input = _serialInArray[0];
  _pid_values_y.input = _serialInArray[1];
}

void FaceDetectionControlLoop::actuate() {
  _angle_x = _angle_x + _pid_values_x.output;
  _angle_y = _angle_y + _pid_values_y.output;

  _servo_x->setDegree(_angle_x);
  _servo_y->setDegree(_angle_y);
}

void FaceDetectionControlLoop::run() {
  Serial.println("run");

  while(Serial.available() == 0); // TODO change to 'if(Serial.available())', so that this loop is non-blocking when no data is being send?
  _serialInArray[_serialCount] = Serial.read();
  _serialCount++;

  if(_serialCount > 1) { // two values received (x, y)
    Serial.print(_serialInArray[0]);
    Serial.print(", ");
    Serial.print(_serialInArray[1]);
    Serial.println();

    observe();
    computeControl();
    actuate();
    _serialCount = 0;
  }
}

void FaceDetectionControlLoop::computeControl() {
  _pid_x->Compute();
  _pid_y->Compute();
}
