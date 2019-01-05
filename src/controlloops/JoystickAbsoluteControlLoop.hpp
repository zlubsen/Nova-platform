#ifndef ControlLoop_Joystick_hpp
#define ControlLoop_Joystick_hpp

#include <actuators/NovaServo.h>
#include <sensors/Joystick.h>
#include <config/HardwareConfig.h>
#include <config/NovaConfig.h>
#include <controlloops/AbstractControlLoop.hpp>
#include <FrequencyTimer.h>

typedef struct {
    float x;
    float y;
  } filter_constant;

typedef struct {
    int x;
    int y;
  } output_degrees;

class JoystickAbsoluteControlLoop : public AbstractControlLoop {
  public:
    JoystickAbsoluteControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig);
    ~JoystickAbsoluteControlLoop();
    virtual void run(NovaProtocolCommand* cmd);

  protected:
    FrequencyTimer* _timer;
    joystick_status _joy_left_input;
    joystick_status _joy_right_input;
    output_degrees _joy_left_output;
    output_degrees _joy_right_output;
    filter_constant _filterconst_left;
    filter_constant _filterconst_right;
    NovaServo *_servo1;
    NovaServo *_servo2;
    NovaServo *_servo3;
    NovaServo *_servo4;
    NovaServo *_servo5;
    void observe();
    virtual void mapInputToRange();
    void filterInput();
    virtual void actuate();
    void actuateStepwiseInputServo(NovaServo* servo);
    int correctJoystickInput(int position, int mid_deviation);

  private:
    Joystick *_joy_left;
    Joystick *_joy_right;
    const int _switch_degree_stepsize = 2; // TODO move to NovaConfig class
};

#endif
