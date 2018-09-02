#ifndef ControlLoop_Joystick_Relative_h
#define ControlLoop_Joystick_Relative_h

#include <actuators/NovaServo.h>
#include <sensors/Joystick.h>
#include <config/HardwareConfig.h>
#include <config/NovaConfig.h>
#include <controlloops/AbstractControlLoop.h>
#include <controlloops/JoystickControlLoop.h>

class JoystickRelativeControlLoop : public JoystickControlLoop {
  public:
    JoystickRelativeControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig);

  private:
    int _max_degrees;
    void mapInputToRange();
    void actuate();
};

#endif