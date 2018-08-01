#include <Communication.h>

#define COMMAND_LENGTH_BYTES 4

// for now we expect a command following this template:
// modcode:opcode:arg1:arg2:arg3<
// where modcode, opcode, arg1-3 are single bytes containing a value (e.g. 5 tokens total)

Communication::Communication() {
  // initialize command array
}

bool Communication::commandAvailable() {
  return (Serial.available() > COMMAND_LENGTH_BYTES);
}

void Communication::parseInput() {
  while(this->commandAvailable() || _commands.count() < MAX_COMMAND_SIZE ) {
    String received = Serial.readStringUntil(MSG_END);
    char received_buf[(received.length()+1)];
    received.toCharArray(received_buf, received.length()+1, 0);
    received_buf[received.length()] = '\0';

    NovaCommand *cmd = new NovaCommand;

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

    _commands.push(*cmd);
  }
}

NovaCommand* Communication::readCommand() {
  return &(_commands.pop());
}

void Communication::writeCommand() {
  // TODO make fit for sending different types of commands; only ACK supported for now
  Serial.println(CMD_ACK);
}
