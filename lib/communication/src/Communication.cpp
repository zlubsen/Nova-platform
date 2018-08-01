#include <Communication.h>

#define COMMAND_LENGTH_BYTES 4

// for now we expect a command following this template:
// modcode:opcode:arg1:arg2:arg3<
// where modcode, opcode, arg1-3 are single bytes containing a value (e.g. 5 tokens total)

Communication::Communication() {
  // initialize command array
  for(int i = 0; i < MAX_COMMAND_SIZE; i++) {
    _commands[i] = NULL;
  }
}

bool Communication::commandAvailable() {
  return (Serial.available() > COMMAND_LENGTH_BYTES);
}

void Communication::parseInput() {
  while(this->commandAvailable() || _command_count <= MAX_COMMAND_SIZE ) {
    String received = Serial.readStringUntil(MSG_END);
    char received_buf[(received.length()+1)];
    received.toCharArray(received_buf, received.length()+1, 0);
    received_buf[received.length()] = '\0';

    nova_command *cmd = new nova_command;

    char *modcode = strtok(received_buf, ":<");
    char *opcode = strtok(NULL, ":<");
    char *arg1 = strtok(NULL, ":<");
    char *arg2 = strtok(NULL, ":<");
    char *arg3 = strtok(NULL, ":<");

    cmd->modulecode = String(modcode).toInt();
    cmd->operandcode = String(opcode).toInt();
    cmd->arg1 = String(arg1).toInt();
    cmd->arg2 = String(arg2).toInt();
    cmd->arg3 = String(arg3).toInt();

    _commands[_command_count] = cmd;
    _command_count++;
  }
}

nova_command* Communication::readCommand() {
  if(_command_count > 0) {
    nova_command *cmd = _commands[0];
    shiftCommandArray();
    return cmd;
  } else
    return NULL;
}

void Communication::shiftCommandArray() {
  // shift all available commands upwards in the array; basically it is a queue
  for(int i = 1; i < MAX_COMMAND_SIZE && i <= _command_count; i++) {
    nova_command *cmdSwap = _commands[i];
    if(cmdSwap != NULL);
    _commands[i-1] = cmdSwap;
  }
  _commands[_command_count] = NULL; // clear last item, it is now located one index above
  _command_count--;
}

void Communication::writeCommand() {
  // TODO make fit for sending different types of commands; only ACK supported for now
  Serial.println(CMD_ACK);
}
