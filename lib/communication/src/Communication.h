#ifndef Nova_Util_Communication_h
#define Nova_Util_Communication_h

#include <Arduino.h>
#include <Queue.h>

typedef struct {
  int modulecode;
  int operandcode;
  int arg1; // operations have at most 3 arguments
  int arg2;
  int arg3;
} NovaCommand;

class Communication {
  public:
    Communication(int baud_rate);
    void run();
    bool commandAvailable();
    void writeCommand();
    void writeCommand(int modcode, int opcode, int arg1, int arg2, int arg3);
    NovaCommand* readCommand();

  private:
    bool serialInputAvailable();
    void parseInput();
    const int MAX_COMMAND_SIZE = 20;
    char MSG_SEPARATOR = ':';
    char MSG_END = '<';
    char CMD_ACK = '&';
    Queue<NovaCommand> _commands = Queue<NovaCommand>(MAX_COMMAND_SIZE);
};

#endif
