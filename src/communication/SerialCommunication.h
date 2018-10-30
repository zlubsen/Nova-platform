#ifndef Nova_Util_Serial_Communication_h
#define Nova_Util_Serial_Communication_h

#include <Arduino.h>
#include <config/NovaConstants.h>
#include <ProtocolLogic.hpp>
#include <queue>
#include <vector>
#include <string>
#include <sstream>

#define serial_num_bytes 32 // We can read a maximum of 32 bytes per loop

class SerialCommunication {
  public:
    SerialCommunication(int baud_rate);
    void run();
    bool commandAvailable();
    void writeCommand(uint8_t module, uint8_t asset, uint8_t operation);
    void writeCommand(uint8_t module, uint8_t asset, uint8_t operation, std::vector<int>* args);
    void writeCommand(NovaProtocolCommand cmd);
    NovaProtocolCommand* readCommand();

  private:
    void parseInput();
    void recvBytesWithStartEndMarkers();
    void sendOutgoingCommands();
    std::queue<NovaProtocolCommand> _commands_in;
    std::queue<NovaProtocolCommand> _commands_out;
    char _receivedBytes[serial_num_bytes];
    char _tempBytes[serial_num_bytes];
    byte _numReceived = 0;
    boolean _newData = false;
};

#endif
