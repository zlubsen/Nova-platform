#include <Communication.h>

#define COMMAND_LENGTH_BYTES 10 // minimal length of a command to nova

// we expect a command following this template:
// modcode:opcode:arg1:arg2:arg3<
// where modcode, opcode, arg1-3 are one to three bytes containing an int value (as string)

Communication::Communication(int baud_rate) {
  // initialize command array
  Serial.begin(baud_rate);
  Serial.setTimeout(500);
}

bool Communication::serialInputAvailable() {
  /*Serial.print("bytes available: ");
  Serial.println(Serial.available());*/
  return (Serial.available() > COMMAND_LENGTH_BYTES);
}

bool Communication::commandAvailable() {
  return _commands.count() > 0;
}

void Communication::parseInput() {
  /*Serial.print("Parse input: ");
  Serial.print(this->serialInputAvailable());
  Serial.print(" & ");
  Serial.println(_commands.count() < MAX_COMMAND_SIZE);*/

  while(this->serialInputAvailable() && _commands.count() < MAX_COMMAND_SIZE) {
    Serial.print("pre read until: ");
    Serial.println(String(Serial.peek()));
    String received = Serial.readStringUntil(MSG_END);
    Serial.println("post read until");

    char received_buf[(received.length()+1)];
    received.toCharArray(received_buf, received.length()+1, 0);
    received_buf[received.length()] = '\0';

    char *modcode = strtok(received_buf, ":");
    char *opcode = strtok(NULL, ":");
    char *arg1 = strtok(NULL, ":");
    char *arg2 = strtok(NULL, ":");
    char *arg3 = strtok(NULL, ":");

    NovaCommand cmd;
    cmd.modulecode = String(modcode).toInt();
    cmd.operandcode = String(opcode).toInt();
    cmd.arg1 = String(arg1).toInt();
    cmd.arg2 = String(arg2).toInt();
    cmd.arg3 = String(arg3).toInt();

    _commands.push(cmd);

    /*Serial.print("Command received: ");
    Serial.print(cmd.modulecode);
    Serial.print(" : ");
    Serial.print(cmd.operandcode);
    Serial.print(" : ");
    Serial.print(cmd.arg1);
    Serial.print(" : ");
    Serial.print(cmd.arg2);
    Serial.print(" : ");
    Serial.println(cmd.arg3);*/
  }
}

NovaCommand* Communication::readCommand() {
  if(commandAvailable()) {
    NovaCommand cmd = _commands.pop();
    // TODO novacommand copy constructor
    NovaCommand *retval = new NovaCommand;
    retval->modulecode = cmd.modulecode;
    retval->operandcode = cmd.operandcode;
    retval->arg1 = cmd.arg1;
    retval->arg2 = cmd.arg2;
    retval->arg3 = cmd.arg3;

    delete &cmd; // cleanup the command that came from the queue

    return retval;
  } else
    return NULL;
}

void Communication::writeCommand() {
  // TODO only ACK supported for now; use parametrised version for other commands
  Serial.println(CMD_ACK);
}

void Communication::writeCommand(int modcode, int opcode, int arg1, int arg2, int arg3) {
  String message = String(modcode);
  message.concat(':');
  message.concat(opcode);
  message.concat(':');
  message.concat(arg1);
  message.concat(':');
  message.concat(arg2);
  message.concat(':');
  message.concat(arg3);
  message.concat('<');

  Serial.println(message);
}

void Communication::run() {
  parseInput();
}
