#include <Arduino.h>
#include "FaceDetectionControlLoop.h"

FaceDetectionControlLoop::FaceDetectionControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig) {
  _comm = hardwareConfig->comm;

  _servo_x = hardwareConfig->servo4;
  _servo_y = hardwareConfig->servo3;

  _pid_config_x = novaConfig->_face_detection_pid_config_x;
  _pid_values_x.setpoint = _servo_x->getDegree();
  setupPIDcontroller(_pid_x, &_pid_config_x, &_pid_values_x);

  _pid_config_y = novaConfig->_face_detection_pid_config_y;
  _pid_values_y.setpoint = _servo_y->getDegree();
  setupPIDcontroller(_pid_y, &_pid_config_y, &_pid_values_y);

  /*Serial.print("pid x: ");
  Serial.print(_pid_x->GetKp());
  Serial.print(" ");
  Serial.print(_pid_x->GetKi());
  Serial.print(" ");
  Serial.println(_pid_x->GetKd());
  Serial.print("pid y: ");
  Serial.print(_pid_x->GetKp());
  Serial.print(" ");
  Serial.print(_pid_x->GetKi());
  Serial.print(" ");
  Serial.println(_pid_x->GetKd());

  Serial.print("default x values: ");
  Serial.print(_pid_values_x.input);
  Serial.print(" ");
  Serial.print(_pid_values_x.setpoint);
  Serial.print(" ");
  Serial.println(_pid_values_x.output);
  Serial.print("default y values: ");
  Serial.print(_pid_values_y.input);
  Serial.print(" ");
  Serial.print(_pid_values_y.setpoint);
  Serial.print(" ");
  Serial.println(_pid_values_y.output);*/

  //dry run to init pid?
  _pid_values_x.input = _servo_x->getDegree();
  _pid_values_x.output = 0.0;
  _pid_x->Compute();
  _pid_values_y.input = _servo_y->getDegree();
  _pid_values_y.output = 0.0;
  _pid_y->Compute();

  /*Serial.print("x on start: ");
  Serial.print(_pid_values_x.input);
  Serial.print(" +/- ");
  Serial.print(_pid_values_x.output);
  Serial.print(" : (default) ");
  Serial.print(_servo_x->getDegree());
  Serial.print(" -> (default) ");
  Serial.println(_angle_x);
  Serial.print("y on start: ");
  Serial.print(_pid_values_y.input);
  Serial.print(" +/- ");
  Serial.print(_pid_values_y.output);
  Serial.print(" : (default) ");
  Serial.print(_servo_y->getDegree());
  Serial.print(" -> (default) ");
  Serial.println(_angle_y);*/
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
