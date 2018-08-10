#include <Communication.h>

// we expect a command following this template:
// >modcode:opcode:arg1:arg2:arg3<
// where modcode, opcode, arg1-3 are bytes containing an int value (as string)

Communication::Communication(int baud_rate) {
  Serial.begin(baud_rate);
}

bool Communication::commandAvailable() {
  return _commands.count() > 0;
}

void Communication::recvBytesWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  byte rb;

  while (Serial.available() > 0 && _newData == false) {
    rb = Serial.read();

    if (recvInProgress == true) {
        if (rb != _endMarker) {
            _receivedBytes[ndx] = rb;
            ndx++;
            if (ndx >= numBytes) {
                ndx = numBytes - 1;
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

void Communication::parseInput() {
  if(_newData) {
    strcpy(_tempBytes, _receivedBytes);

    NovaCommand cmd;
    cmd.modulecode = atoi(strtok(_tempBytes, ":"));
    cmd.operandcode = atoi(strtok(NULL, ":"));
    cmd.arg1 = atoi(strtok(NULL, ":"));
    cmd.arg2 = atoi(strtok(NULL, ":"));
    cmd.arg3 = atoi(strtok(NULL, ":"));

    _commands.push(cmd);
    _newData = false;
  }
}

NovaCommand* Communication::readCommand() {
  if(commandAvailable()) {
    NovaCommand cmd = _commands.pop();
    // TODO novacommand copy constructor?
    NovaCommand *retval = new NovaCommand;

    retval->modulecode = cmd.modulecode;
    retval->operandcode = cmd.operandcode;
    retval->arg1 = cmd.arg1;
    retval->arg2 = cmd.arg2;
    retval->arg3 = cmd.arg3;

    return retval;
  } else
    return NULL;
}

void Communication::writeCommand(int modcode, int opcode, int arg1, int arg2, int arg3) {
  String message = String(CMD_START_MARKER);
  message.concat(modcode);
  message.concat(CMD_SEPARATOR);
  message.concat(opcode);
  message.concat(CMD_SEPARATOR);
  message.concat(arg1);
  message.concat(CMD_SEPARATOR);
  message.concat(arg2);
  message.concat(CMD_SEPARATOR);
  message.concat(arg3);
  message.concat(CMD_END);

  Serial.print(message);
}

void Communication::run() {
  recvBytesWithStartEndMarkers();
  parseInput();
}
