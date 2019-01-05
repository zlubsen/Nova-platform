#ifndef ControlLoop_ExternalInput_hpp
#define ControlLoop_ExternalInput_hpp

#include <actuators/NovaServo.h>
#include <config/HardwareConfig.h>
#include <config/NovaConfig.h>
#include <controlloops/AbstractControlLoop.hpp>

class ExternalInputControlLoop : public AbstractControlLoop {
  public:
    ExternalInputControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig);
    ~ExternalInputControlLoop() {}
    void run(NovaProtocolCommand* cmd);

  private:
    NovaServo *_servo1;
    NovaServo *_servo2;
    NovaServo *_servo3;
    NovaServo *_servo4;
    NovaServo *_servo5;
    int _offset_axis_1 = 0;
    int _offset_axis_2 = 0;
    int _offset_axis_3 = 0;
    int _offset_axis_4 = 0;
    int _offset_axis_5 = 0;
    void observe(NovaProtocolCommand* cmd);
    void actuate();
};

#endif
