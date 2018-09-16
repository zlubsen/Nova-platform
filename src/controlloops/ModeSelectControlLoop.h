#ifndef ControlLoop_Mode_Select_h
#define ControlLoop_Mode_Select_h

#include "config/HardwareConfig.h"
#include "config/NovaConfig.h"
#include <controlloops/AbstractControlLoop.h>

class ModeSelectControlLoop : public AbstractControlLoop {
  public:
    ModeSelectControlLoop(HardwareConfig*  hardwareConfig, NovaConfig* novaConfig);
    void run(NovaCommand*);
  private:
};

#endif
