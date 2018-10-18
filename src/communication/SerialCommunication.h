#ifndef Nova_Util_Serial_Communication_h
#define Nova_Util_Serial_Communication_h

#include <Arduino.h>
#include <config/NovaConstants.h>
#include <queue>
#include <string>
#include <sstream>

#define serial_num_bytes 32 // We can read a maximum of 32 bytes per loop

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
    void sendOutgoingCommands();
    std::queue<NovaCommand> _commands_in;
    std::queue<NovaCommand> _commands_out;
    char _receivedBytes[serial_num_bytes];
    char _tempBytes[serial_num_bytes];
    byte _numReceived = 0;
    boolean _newData = false;
};

#endif
