#ifndef ControlLoop_Distance_Avoid_h
#define ControlLoop_Distance_Avoid_h

#include "sensors/UltraSoundSensor.h"
#include "config/HardwareConfig.h"
#include "config/NovaConfig.h"
#include <PID_v1.h>
#include <controlloops/AbstractControlLoop.h>
#include <ProtocolLogic.hpp>
#include <string>

class DistanceAvoidControlLoop : public AbstractControlLoop {
  public:
    DistanceAvoidControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig);
    ~DistanceAvoidControlLoop();
    void run(NovaProtocolCommand* cmd);
    std::string getLCDStatusString();

  private:
    SerialCommunication* _comm;
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
    void handleCommands(NovaProtocolCommand* cmd);
    void setMinimumDistanceLimit(int new_distance);
    void setMaximumDistanceLimit(int new_distance);
    void setSetpoint(int new_distance);
    void setPIDTuning(int p_value, int i_value, int d_value);
    void computeControl();
    void observe();
    void actuate();
    void statusPublishPIDValues();
};

#endif
