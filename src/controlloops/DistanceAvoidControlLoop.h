#ifndef ControlLoop_Distance_Avoid_h
#define ControlLoop_Distance_Avoid_h

#include "sensors/UltraSoundSensor.h"
#include "config/HardwareConfig.h"
#include "config/NovaConfig.h"
#include <PID_v1.h>
#include <controlloops/AbstractControlLoop.h>

class DistanceAvoidControlLoop : public AbstractControlLoop {
  public:
    DistanceAvoidControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig);
    void run(NovaCommand* cmd);

  private:
    UltraSoundSensor *_ultraSoundSensor;
    NovaServo *_servo;
    PID *_pid;
    pid_config _pid_config;
    pid_dynamic_values _pid_values;
    double _setpoint_distance;
    double _input_distance;
    double _output_distance;
    int _max_distance;
    int _min_distance;
    int _servo_angle;
    void setupPIDcontroller();
    void handleCommands(NovaCommand* cmd);
    void setDistanceLimit(bool set_minimum, int new_distance);
    void setSetpoint(int new_distance);
    void setPIDTuning(int p_value, int i_value, int d_value);
    void computeControl();
    void observe();
    void actuate();
};

#endif