#ifndef ControlLoop_Abstract_h
#define ControlLoop_Abstract_h

#include <string>

class AbstractControlLoop {
  public:
    virtual ~AbstractControlLoop() {}
    virtual void run(NovaProtocolCommand* cmd) = 0;
    virtual std::string getLCDStatusString() = 0;
};

#endif
