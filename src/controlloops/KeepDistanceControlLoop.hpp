#ifndef ControlLoop_Keep_Distance_hpp
#define ControlLoop_Keep_Distance_hpp

#include "sensors/UltraSoundSensor.h"
#include "config/HardwareConfig.h"
#include "config/NovaConfig.h"
#include <PID_v1.h>
#include <controlloops/AbstractControlLoop.hpp>
#include <ProtocolLogic.hpp>
#include <string>

class KeepDistanceControlLoop : public AbstractControlLoop {
  public:
    KeepDistanceControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig);
    ~KeepDistanceControlLoop();
    void run(NovaProtocolCommand* cmd);
    std::vector<std::string> getLCDStatusList();

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
    std::string getDistanceMessage();
};

#endif
