#include <Arduino.h>
#include "TrackObjectControlLoop.hpp"
#include <MemoryFree.hpp>

TrackObjectControlLoop::TrackObjectControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig) {
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
  setupStatusMessages();
}

TrackObjectControlLoop::~TrackObjectControlLoop() {
    delete _pid_x;
    delete _pid_y;
}

// TODO: this one does not work well, pointers given to the PID are not passed correctly...
void TrackObjectControlLoop::setupPIDcontroller(PID* pid, pid_config* config, pid_dynamic_values* values) {
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

void TrackObjectControlLoop::setupStatusMessages() {
  _status_messages.push_back(valueToLCDString("X Kp", _pid_x->GetKp()));
  _status_messages.push_back(valueToLCDString("X Ki", _pid_x->GetKi()));
  _status_messages.push_back(valueToLCDString("X Kd", _pid_x->GetKd()));
  _status_messages.push_back(valueToLCDString("Y Kp", _pid_y->GetKp()));
  _status_messages.push_back(valueToLCDString("Y Ki", _pid_y->GetKi()));
  _status_messages.push_back(valueToLCDString("Y Kd", _pid_y->GetKd()));
  _status_messages.push_back(getFreeMemoryMessage());
}

void TrackObjectControlLoop::setSetpoint(uint8_t asset, int new_setpoint) {
  // TODO validate that the setpoint is in a valid range.
  if(asset == cmd_pid_x)
    _pid_values_x.setpoint = new_setpoint;
  else if (asset == cmd_pid_y) {
    _pid_values_y.setpoint = new_setpoint;
  }
}

void TrackObjectControlLoop::setPIDTuning(uint8_t asset, int p_value, int i_value, int d_value) {
  double new_p = ((double) p_value)/1000;
  double new_i = ((double) i_value)/1000;
  double new_d = ((double) d_value)/1000;

  if(asset == cmd_pid_x)
    _pid_x->SetTunings(new_p, new_i, new_d);
  else if(asset == cmd_pid_y)
    _pid_y->SetTunings(new_p, new_i, new_d);

  statusPublishPIDValues();
}

// TODO refactor pid_x and pid_y into reusable method
void TrackObjectControlLoop::statusPublishPIDValues() {
  int Kp_x = (int)(_pid_x->GetKp()*1000);
  int Ki_x = (int)(_pid_x->GetKi()*1000);
  int Kd_x = (int)(_pid_x->GetKd()*1000);
  std::vector<int> x_args {Kp_x, Ki_x, Kd_x};

  NovaProtocolCommandBuilder* builder_x = _comm->getBuilder();

  _comm->writeCommand(builder_x
    ->setModule(cmd_track_object)
    ->setAsset(cmd_pid_x)
    ->setOperation(cmd_get_tuning)
    ->setArgs(x_args)
    ->build());

  int Kp_y = (int)(_pid_y->GetKp()*1000);
  int Ki_y = (int)(_pid_y->GetKi()*1000);
  int Kd_y = (int)(_pid_y->GetKd()*1000);
  std::vector<int> y_args {Kp_y, Ki_y, Kd_y};

  NovaProtocolCommandBuilder* builder_y = _comm->getBuilder();

  _comm->writeCommand(builder_y
    ->setModule(cmd_track_object)
    ->setAsset(cmd_pid_y)
    ->setOperation(cmd_get_tuning)
    ->setArgs(y_args)
    ->build());
}

void TrackObjectControlLoop::observe(NovaProtocolCommand *cmd) {
  if(cmd->args.size() == 2) {
    _pid_values_x.input = cmd->args.at(0);
    _pid_values_y.input = cmd->args.at(1);
  }
}

void TrackObjectControlLoop::computeControl() {
  _pid_x->Compute();
  _pid_y->Compute();
}

void TrackObjectControlLoop::actuate() {
  int angle_x = _servo_x->getDegree() + _pid_values_x.output;
  int angle_y = _servo_y->getDegree() + _pid_values_y.output;

  _servo_x->setDegree(angle_x);
  _servo_y->setDegree(angle_y);
}

void TrackObjectControlLoop::handleCommands(NovaProtocolCommand* cmd) {
  if(cmd->operation == cmd_set_tuning && cmd->args.size() == 3)
    setPIDTuning(cmd->asset, cmd->args.at(0), cmd->args.at(1), cmd->args.at(2));
  else if(cmd->operation == cmd_set_setpoint && cmd->args.size() == 1)
    setSetpoint(cmd->asset, cmd->args.at(0));
}

void TrackObjectControlLoop::run(NovaProtocolCommand* cmd) {
  if(cmd != nullptr && cmd->module == cmd_track_object) {
    handleCommands(cmd);

    if(cmd->asset == cmd_module && cmd->operation == cmd_set_coordinates) {
      observe(cmd);
      computeControl();
      actuate();

      NovaProtocolCommandBuilder* builder = _comm->getBuilder();
      _comm->writeCommand(builder
        ->setModule(cmd_track_object)
        ->setAsset(cmd_module)
        ->setOperation(cmd_ack_coordinates)
        ->build());
    }
  }
}

void TrackObjectControlLoop::updateStatusMessages() {
  switch (_current_status_message_index) {
    case 0:
      _status_messages[0] = valueToLCDString("X Kp", _pid_x->GetKp());
      break;
    case 1:
      _status_messages[1] = valueToLCDString("X Ki", _pid_x->GetKi());
      break;
    case 2:
      _status_messages[2] = valueToLCDString("X Kd", _pid_x->GetKd());
      break;
    case 3:
      _status_messages[3] = valueToLCDString("Y Kp", _pid_y->GetKp());
      break;
    case 4:
      _status_messages[4] = valueToLCDString("Y Ki", _pid_y->GetKi());
      break;
    case 5:
      _status_messages[5] = valueToLCDString("Y Kd", _pid_y->GetKd());
      break;
    case 6:
      _status_messages[6] = getFreeMemoryMessage();
      break;
  }
}
