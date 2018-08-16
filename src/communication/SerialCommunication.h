#ifndef Nova_Util_Serial_Communication_h
#define Nova_Util_Serial_Communication_h

#include <Arduino.h>
#include <Queue.h>
#include <config/NovaConstants.h>

#define numBytes 32 // We can read a maximum of 32 bytes per loop

typedef struct {
  int modulecode;
  int operandcode;
  int arg1; // commands have at most 3 arguments
  int arg2;
  int arg3;
} NovaCommand;

class SerialCommunication {
  public:
    SerialCommunication(int baud_rate);
    void run();
    bool commandAvailable();
    void writeCommand(int modcode, int opcode, int arg1, int arg2, int arg3);
    NovaCommand* readCommand();

  private:
    void parseInput();
    void recvBytesWithStartEndMarkers();
    void sendOutGoingCommands();
    const int MAX_COMMAND_SIZE = 20; // we can store a maximum of 20 commands per loop (which won't fit in 32 bytes anyhow - see 'define numBytes')
    //char CMD_SEPARATOR = ':';
    //char CMD_START_MARKER = '>';
    //char CMD_END_MARKER = '<';
    Queue<NovaCommand> _commands_in = Queue<NovaCommand>(MAX_COMMAND_SIZE);
    Queue<NovaCommand> _commands_out = Queue<NovaCommand>(MAX_COMMAND_SIZE);
    char _startMarker = 0x3E; // '>' TODO: there is redundancy in using both char and byte version of the markers/msg-indicators
    char _endMarker = 0x3C; // '<'
    char _receivedBytes[numBytes];
    char _tempBytes[numBytes];
    byte _numReceived = 0;
    boolean _newData = false;
};

#endif
