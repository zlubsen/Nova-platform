#include "SerialCommunication.h"

// we expect a command following this template:
// >module_code:asset_code:operation_code:arg_count:[args_list]<
// all values are uint8_t
// args_list is a list of integers, separated by colons

SerialCommunication::SerialCommunication(int baud_rate) {
  Serial.begin(baud_rate);

  builder = new NovaProtocolCommandBuilder(&root_node);
  reader = new NovaProtocolCommandReader(&root_node);
}

NovaProtocolCommandBuilder* SerialCommunication::getBuilder() {
  return builder;
}

bool SerialCommunication::commandAvailable() {
  return _commands_in.size() > 0;
}

void SerialCommunication::recvBytesWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  byte rb;

  while (Serial.available() > 0 && _newData == false) {
    rb = Serial.read();

    if (recvInProgress == true) {
        if (rb != NovaConstants::CMD_END_MARKER) {
            _receivedBytes[ndx] = rb;
            ndx++;
            if (ndx >= serial_num_bytes) {
                ndx = serial_num_bytes - 1;
            }
        }
        else {
            _receivedBytes[ndx] = '\0'; // terminate the string
            recvInProgress = false;
            _numReceived = ndx;  // save the number for use when printing
            ndx = 0;
            _newData = true;
        }
    }
    else if (rb == NovaConstants::CMD_START_MARKER) {
        recvInProgress = true;
    }
  }
}

void SerialCommunication::parseInput() {
  if(_newData) {
    strcpy(_tempBytes, _receivedBytes);
    char *val;

    std::vector<int> received;

    received.push_back(atoi(strtok(_tempBytes, ":")));
    while((val = strtok(NULL, ":")) != NULL) {
      received.push_back(atoi(val));
    }

    NovaProtocolCommand cmd;
    reader->readCommand(&received, &cmd);

    _commands_in.push(cmd);
    _newData = false;
  }
}

NovaProtocolCommand* SerialCommunication::readCommand() {
  if(commandAvailable()) {
    NovaProtocolCommand cmd = _commands_in.front();
    _commands_in.pop();
    NovaProtocolCommand *retval = new NovaProtocolCommand;

    retval->module = cmd.module;
    retval->asset = cmd.asset;
    retval->operation = cmd.operation;
    retval->args = cmd.args;

    return retval;
  } else
    return nullptr;
}

// TODO remove
void SerialCommunication::writeCommand(uint8_t module, uint8_t asset, uint8_t operation) {
  NovaProtocolCommand cmd;

  cmd.module = (int)module;
  cmd.asset = (int)asset;
  cmd.operation = (int8_t)operation;

  _commands_out.push(cmd);
}

// TODO remove
void SerialCommunication::writeCommand(uint8_t module, uint8_t asset, uint8_t operation, int single_argument) {
  NovaProtocolCommand cmd;

  cmd.module = (int)module;
  cmd.asset = (int)asset;
  cmd.operation = (int)operation;
  cmd.args.push_back(single_argument);

  _commands_out.push(cmd);
}

// TODO remove
void SerialCommunication::writeCommand(uint8_t module, uint8_t asset, uint8_t operation, std::vector<int>* args) {
  NovaProtocolCommand cmd;

  cmd.module = (int)module;
  cmd.asset = (int)asset;
  cmd.operation = (int)operation;
  cmd.args = *args;

  _commands_out.push(cmd);
}

void SerialCommunication::writeCommand(NovaProtocolCommand cmd) {
  _commands_out.push(cmd);
}

void SerialCommunication::writeCommand(std::vector<int>* cmd_vector) {
  NovaProtocolCommand cmd;
  cmd.module = cmd_vector->at(0);
  cmd.asset = cmd_vector->at(1);
  cmd.operation = cmd_vector->at(2);
  int argcount = cmd_vector->at(3);
  for(int i = 4; i < (argcount+4); i++) {
    cmd.args.push_back(cmd_vector->at(i));
  }
  writeCommand(cmd);
}

void SerialCommunication::sendOutgoingCommands() {
  while(_commands_out.size() > 0) {
    NovaProtocolCommand cmd = _commands_out.front();
    _commands_out.pop();

    std::stringstream s;
    s << NovaConstants::CMD_START_MARKER
      << cmd.module
      << NovaConstants::CMD_SEPARATOR
      << cmd.asset
      << NovaConstants::CMD_SEPARATOR
      << cmd.operation
      << NovaConstants::CMD_SEPARATOR
      << cmd.args.size();
      for(int arg : cmd.args) {
        s << NovaConstants::CMD_SEPARATOR
        << arg;
      }
      s << NovaConstants::CMD_END_MARKER;

    std::string message = s.str();
    Serial.print(message.c_str());
  }
}

void SerialCommunication::run() {
  sendOutgoingCommands();
  recvBytesWithStartEndMarkers();
  parseInput();
}
