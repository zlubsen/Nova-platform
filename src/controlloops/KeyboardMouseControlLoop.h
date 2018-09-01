#ifndef ControlLoop_KeyboardMouse_h
#define ControlLoop_KeyboardMouse_h

#include <actuators/NovaServo.h>
#include <config/HardwareConfig.h>
#include <config/NovaConfig.h>
#include <controlloops/AbstractControlLoop.h>

class KeyboardMouseControlLoop : public AbstractControlLoop {
  public:
    KeyboardMouseControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig);
    void run(NovaCommand* cmd);

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
    void observe(NovaCommand* cmd);
    void actuate();
};

#endif
