#include "SerialCommunication.h"

// we expect a command following this template:
// >modcode:opcode:arg1:arg2:arg3<
// where modcode, opcode, arg1-3 are bytes containing an int value (as string)

SerialCommunication::SerialCommunication(int baud_rate) {
  Serial.begin(baud_rate);
}

bool SerialCommunication::commandAvailable() {
  return _commands_in.count() > 0;
}

void SerialCommunication::recvBytesWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  byte rb;

  while (Serial.available() > 0 && _newData == false) {
    rb = Serial.read();

    if (recvInProgress == true) {
        if (rb != _endMarker) {
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
    else if (rb == _startMarker) {
        recvInProgress = true;
    }
  }
}

void SerialCommunication::parseInput() {
  if(_newData) {
    strcpy(_tempBytes, _receivedBytes);

    NovaCommand cmd;
    cmd.modulecode = atoi(strtok(_tempBytes, ":"));
    cmd.operandcode = atoi(strtok(NULL, ":"));
    cmd.arg1 = atoi(strtok(NULL, ":"));
    cmd.arg2 = atoi(strtok(NULL, ":"));
    cmd.arg3 = atoi(strtok(NULL, ":"));

    _commands_in.push(cmd);
    _newData = false;
  }
}

NovaCommand* SerialCommunication::readCommand() {
  if(commandAvailable()) {
    NovaCommand cmd = _commands_in.pop();
    // TODO novacommand copy constructor?
    NovaCommand *retval = new NovaCommand;

    retval->modulecode = cmd.modulecode;
    retval->operandcode = cmd.operandcode;
    retval->arg1 = cmd.arg1;
    retval->arg2 = cmd.arg2;
    retval->arg3 = cmd.arg3;

    return retval;
  } else
    return nullptr;
}

void SerialCommunication::writeCommand(int modcode, int opcode, int arg1, int arg2, int arg3) {
  NovaCommand cmd;
  cmd.modulecode = modcode;
  cmd.operandcode = opcode;
  cmd.arg1 = arg1;
  cmd.arg2 = arg2;
  cmd.arg3 = arg3;

  _commands_out.push(cmd);
}

void SerialCommunication::sendOutgoingCommands() {
  while(_commands_out.count() > 0) {
    NovaCommand cmd = _commands_out.pop();

    String message = String(NovaConstants::CMD_START_MARKER);
    message.concat(cmd.modulecode);
    message.concat(NovaConstants::CMD_SEPARATOR);
    message.concat(cmd.operandcode);
    message.concat(NovaConstants::CMD_SEPARATOR);
    message.concat(cmd.arg1);
    message.concat(NovaConstants::CMD_SEPARATOR);
    message.concat(cmd.arg2);
    message.concat(NovaConstants::CMD_SEPARATOR);
    message.concat(cmd.arg3);
    message.concat(NovaConstants::CMD_END_MARKER);

    Serial.print(message);
  }
}

void SerialCommunication::run() {
  sendOutgoingCommands();
  recvBytesWithStartEndMarkers();
  parseInput();
}
