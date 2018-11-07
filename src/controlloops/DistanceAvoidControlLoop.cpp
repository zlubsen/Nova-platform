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

DistanceAvoidControlLoop::~DistanceAvoidControlLoop() {
  delete _pid;
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

void DistanceAvoidControlLoop::handleCommands(NovaProtocolCommand* cmd) {
  switch (cmd->operation) {
    case cmd_set_min_distance:
      setMinimumDistanceLimit(cmd->args.at(0));
      break;
    case cmd_set_max_distance:
      setMaximumDistanceLimit(cmd->args.at(0));
      break;
    case cmd_set_setpoint:
      setSetpoint(cmd->args.at(0));
      break;
    case cmd_set_tuning:
      if(cmd->asset == cmd_pid && cmd->args.size() == 3)
        setPIDTuning(cmd->args.at(0), cmd->args.at(1), cmd->args.at(3));
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
  std::vector<int> args {Kp, Ki, Kd};

  NovaProtocolCommandBuilder* builder = _comm->getBuilder();

  _comm->writeCommand(builder
    ->setModule(cmd_keep_distance)
    ->setAsset(cmd_pid)
    ->setOperation(cmd_get_tuning)
    ->setArgs(args)
    ->build());
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

void DistanceAvoidControlLoop::run(NovaProtocolCommand* cmd) {
  if(cmd != nullptr && cmd->module == cmd_keep_distance) {
    handleCommands(cmd);
  }
  observe();
  computeControl();
  actuate();
}

// TODO: write the static text once (str_start, str_end), and update only the changing number/distance
std::string DistanceAvoidControlLoop::getLCDStatusString() {
  std::string str_start = "Dist:";
  std::string str_end = " cm";

  char buffer[3];
  sprintf(buffer, "%d", (int)_pid_values.input);
  std::string str_value(buffer);

  int text_length = str_start.size() + str_value.size() + str_end.size();
  std::string mid_padding(16-text_length, ' ');

  std::stringstream s;
  s << str_start << mid_padding << str_value << str_end;

  return s.str();
}
