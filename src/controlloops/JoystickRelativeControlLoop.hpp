#ifndef ControlLoop_Joystick_Relative_hpp
#define ControlLoop_Joystick_Relative_hpp

#include <actuators/NovaServo.h>
#include <sensors/Joystick.h>
#include <config/HardwareConfig.h>
#include <config/NovaConfig.h>
#include <controlloops/AbstractControlLoop.hpp>
#include <controlloops/JoystickAbsoluteControlLoop.hpp>

class JoystickRelativeControlLoop : public JoystickAbsoluteControlLoop {
  public:
    JoystickRelativeControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig);
    void run(NovaProtocolCommand* cmd);
    //std::string getLCDStatusString();

  protected:
    void mapInputToRange();
    void actuate();

  private:
    int _max_degrees;
};

#endif
