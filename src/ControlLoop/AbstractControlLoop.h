#ifndef ControlLoop_Abstract_h
#define ControlLoop_Abstract_h

class AbstractControlLoop {
  public:
    virtual void run(NovaCommand* cmd) = 0;
};

#endif
