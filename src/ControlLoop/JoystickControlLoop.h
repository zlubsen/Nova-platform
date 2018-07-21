#ifndef ControlLoop_Joystick_h
#define ControlLoop_Joystick_h

#include <NovaServo.h>
#include <Joystick.h>
#include <HardwareConfig.h>
#include <NovaConfig.h>

typedef struct {
    float x;
    float y;
  } filter_constant;

typedef struct {
    int x;
    int y;
  } output_degrees;

class JoystickControlLoop {
  public:
    JoystickControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig);
    void observe();
    void actuate();
    void run();
    joystick_status _joy_left_input;
    joystick_status _joy_right_input;
    output_degrees _joy_left_output;
    output_degrees _joy_right_output;
    filter_constant _filterconst_left;
    filter_constant _filterconst_right;

  private:
    Joystick *_joy_left;
    Joystick *_joy_right;
     /*joystick_status _joy_left_input;
    joystick_status _joy_right_input;
    output_degrees _joy_left_output;
    output_degrees _joy_right_output;
    filter_constant _filterconst_left;
    filter_constant _filterconst_right;*/
    NovaServo *_servo1;
    NovaServo *_servo2;
    NovaServo *_servo3;
    NovaServo *_servo4;
    NovaServo *_servo5;
    void actuateContinousInputServo(NovaServo* servo, int degree);
    void actuateStepwiseInputServo(NovaServo* servo);
    void mapInputToRange();
    void filterInput();
    const int _switch_degree_stepsize = 2;
};

#endif
