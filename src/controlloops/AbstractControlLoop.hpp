#ifndef ControlLoop_Abstract_hpp
#define ControlLoop_Abstract_hpp

#include <Protocol.hpp>
#include <string>
#include <vector>

class AbstractControlLoop {
  public:
    AbstractControlLoop();
    virtual ~AbstractControlLoop() {}
    virtual void run(NovaProtocolCommand* cmd) = 0;
    void cycleStatusMessageNext();
    void cycleStatusMessagePrevious();
    virtual std::string getLCDStatusMessage();
  protected:
    std::vector<std::string> _status_messages;
    uint8_t _current_status_message_index = 0;
    std::string getFreeMemoryString();
    std::string valueToLCDString(std::string item, double value);
};

#endif
