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
  _pid_values_y.output = 0.0; // explicitly set output to zero, otherwise there appears to be garbage in the memory...

  statusPublishPIDValues();
}

// TODO: this one does not work well, pointers given to the PID are not passed correctly...
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

void FaceDetectionControlLoop::setSetpoint(int opcode, int new_setpoint) {
  // TODO validate that the setpoint is in a valid range.
  if(opcode == NovaConstants::OP_FACE_DETECTION_X_SETPOINT)
    _pid_values_x.setpoint = new_setpoint;
  else if (opcode == NovaConstants::OP_FACE_DETECTION_Y_SETPOINT) {
    _pid_values_y.setpoint = new_setpoint;
  }
}

void FaceDetectionControlLoop::setPIDTuning(int opcode, int p_value, int i_value, int d_value) {
  double new_p = ((double) p_value)/1000;
  double new_i = ((double) i_value)/1000;
  double new_d = ((double) d_value)/1000;

  if(opcode == NovaConstants::OP_FACE_DETECTION_X_PID_TUNING)
    _pid_x->SetTunings(new_p, new_i, new_d);
  else if(opcode == NovaConstants::OP_FACE_DETECTION_Y_PID_TUNING)
    _pid_y->SetTunings(new_p, new_i, new_d);

  statusPublishPIDValues();
}

void FaceDetectionControlLoop::statusPublishPIDValues() {
  int Kp_x = (int)(_pid_x->GetKp()*1000);
  int Ki_x = (int)(_pid_x->GetKi()*1000);
  int Kd_x = (int)(_pid_x->GetKd()*1000);
  _comm->writeCommand(NovaConstants::MOD_STATUS_NOVA, NovaConstants::OP_STATUS_RECEIVE_FACEDETECT_PID_X, Kp_x, Ki_x, Kd_x);

  int Kp_y = (int)(_pid_y->GetKp()*1000);
  int Ki_y = (int)(_pid_y->GetKi()*1000);
  int Kd_y = (int)(_pid_y->GetKd()*1000);
  _comm->writeCommand(NovaConstants::MOD_STATUS_NOVA, NovaConstants::OP_STATUS_RECEIVE_FACEDETECT_PID_Y, Kp_y, Ki_y, Kd_y);
}

void FaceDetectionControlLoop::observe(NovaCommand *cmd) {
  _pid_values_x.input = cmd->arg1;
  _pid_values_y.input = cmd->arg2;
}

void FaceDetectionControlLoop::computeControl() {
  _pid_x->Compute();
  _pid_y->Compute();
}

void FaceDetectionControlLoop::actuate() {
  int angle_x = _servo_x->getDegree() + _pid_values_x.output;
  int angle_y = _servo_y->getDegree() + _pid_values_y.output;

  _servo_x->setDegree(angle_x);
  _servo_y->setDegree(angle_y);
}

void FaceDetectionControlLoop::handleCommands(NovaCommand* cmd) {
  if(cmd->modulecode == NovaConstants::MOD_FACE_DETECTION) {
    if(cmd->operandcode == NovaConstants::OP_FACE_DETECTION_X_PID_TUNING
      || cmd->operandcode == NovaConstants::OP_FACE_DETECTION_Y_PID_TUNING) {
        setPIDTuning(cmd->operandcode, cmd->arg1, cmd->arg2, cmd->arg3);
    }
    if(cmd->operandcode == NovaConstants::OP_FACE_DETECTION_X_SETPOINT
      || cmd->operandcode == NovaConstants::OP_FACE_DETECTION_Y_SETPOINT) {
        setSetpoint(cmd->operandcode, cmd->arg1);
    }
  }
}

void FaceDetectionControlLoop::run(NovaCommand* cmd) {
  if(cmd != NULL && cmd->modulecode == NovaConstants::MOD_FACE_DETECTION) {
    handleCommands(cmd);

    if(cmd->operandcode == NovaConstants::OP_FACE_DETECTION_SET_COORDINATES) {
      observe(cmd);
      computeControl();
      actuate();

      _comm->writeCommand(
        NovaConstants::MOD_FACE_DETECTION,
        NovaConstants::OP_FACE_DETECTION_ACK_COORDINATES,
        0,0,0);
    }
  }
}
