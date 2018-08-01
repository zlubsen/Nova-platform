#ifndef Nova_Util_Communication_h
#define Nova_Util_Communication_h

#include <Arduino.h>

const int MAX_COMMAND_SIZE = 10;

typedef struct {
  int modulecode;
  int operandcode;
  int arg1; // operations have at most 3 arguments
  int arg2;
  int arg3;
} nova_command;

class Communication {
  public:
    Communication();
    bool commandAvailable();
    void parseInput();
    void writeCommand();
    nova_command* readCommand();

  private:
    void shiftCommandArray();
    nova_command *_commands[MAX_COMMAND_SIZE]; // basically used as a queue
    int _command_count = 0;
    //char MSG_START = '>';
    char MSG_SEPARATOR = ':';
    char MSG_END = '<';
    char CMD_ACK = '&';
    //String token_symbols = String(MSG_SEPARATOR) + String(MSG_END);
};

#endif
