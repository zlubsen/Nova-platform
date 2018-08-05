#include <Arduino.h>
#include "FaceDetectionControlLoop.h"

FaceDetectionControlLoop::FaceDetectionControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig) {
  _comm = hardwareConfig->comm;

  _servo_x = hardwareConfig->servo4;
  _servo_y = hardwareConfig->servo3;

  _pid_config_x = novaConfig->_face_detection_pid_config_x;
  _pid_values_x.setpoint = _servo_x->getDegree();
  //setupPIDcontroller(_pid_x, &_pid_config_x, &_pid_values_x);
  _pid_x = new PID(&(_pid_values_x.input),
    &(_pid_values_x.output),
    &(_pid_values_x.setpoint),
    _pid_config_x.Kp,
    _pid_config_x.Ki,
    _pid_config_x.Kd,
    _pid_config_x.direction);

    _pid_x->SetSampleTime(_pid_config_x.sampleTime);
    _pid_x->SetOutputLimits(_pid_config_x.outputLimitMin, _pid_config_x.outputLimitMax);
    _pid_x->SetMode(_pid_config_x.mode);
  _pid_values_x.output = 0.0; // explicitly set output to zero, otherwise there is garbage in the memory...

  _pid_config_y = novaConfig->_face_detection_pid_config_y;
  _pid_values_y.setpoint = _servo_y->getDegree();
  //setupPIDcontroller(_pid_y, &_pid_config_y, &_pid_values_y);
  _pid_y = new PID(&(_pid_values_y.input),
    &(_pid_values_y.output),
    &(_pid_values_y.setpoint),
    _pid_config_y.Kp,
    _pid_config_y.Ki,
    _pid_config_y.Kd,
    _pid_config_y.direction);

    _pid_y->SetSampleTime(_pid_config_y.sampleTime);
    _pid_y->SetOutputLimits(_pid_config_y.outputLimitMin, _pid_config_y.outputLimitMax);
    _pid_y->SetMode(_pid_config_y.mode);
  _pid_values_y.output = 0.0; // explicitly set output to zero, otherwise there is garbage in the memory...
}

void FaceDetectionControlLoop::setupPIDcontroller(PID* pid, pid_config* config, pid_dynamic_values* values) {
  pid = new PID(&(values->input),
    &(values->output),
    &(values->setpoint),
    config->Kp,
    config->Ki,
    config->Kd,
    config->direction);

    pid->SetSampleTime(config->sampleTime);
    pid->SetOutputLimits(config->outputLimitMin, config->outputLimitMax);
    pid->SetMode(config->mode);
}

void FaceDetectionControlLoop::observe(NovaCommand *cmd) {
  _pid_values_x.input = cmd->arg1;
  _pid_values_y.input = cmd->arg2;
  delete cmd;
}

void FaceDetectionControlLoop::computeControl() {
  _pid_x->Compute();
  _pid_y->Compute();
}

void FaceDetectionControlLoop::actuate() {
  int angle_x = _servo_x->getDegree() + _pid_values_x.output;
  int angle_y = _servo_y->getDegree() + _pid_values_y.output;

  Serial.print("x: ");
  Serial.print(_pid_values_x.input);
  Serial.print(" +/- ");
  Serial.print(_pid_values_x.output);
  Serial.print(" : ");
  Serial.print(_servo_x->getDegree());
  Serial.print(" -> ");
  Serial.println(angle_x);

  Serial.print("y: ");
  Serial.print(_pid_values_y.input);
  Serial.print(" +/- ");
  Serial.print(_pid_values_y.output);
  Serial.print(" : ");
  Serial.print(_servo_y->getDegree());
  Serial.print(" -> ");
  Serial.println(angle_y);

  _servo_x->setDegree(angle_x);
  _servo_y->setDegree(angle_y);
}

void FaceDetectionControlLoop::run(NovaCommand* cmd) {
  if(cmd != NULL) {
    observe(cmd);
    computeControl();
    actuate();

    _comm->writeCommand(); // TODO placeholder ACK for now
  }
}
