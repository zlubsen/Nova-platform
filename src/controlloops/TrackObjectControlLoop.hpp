#ifndef ControlLoop_Track_Object_h
#define ControlLoop_Track_Object_h

#include <config/HardwareConfig.h>
#include <config/NovaConfig.h>
#include <config/NovaConstants.h>
#include <PID_v1.h>
#include <controlloops/AbstractControlLoop.hpp>
#include <ProtocolLogic.hpp>

class TrackObjectControlLoop : public AbstractControlLoop {
  public:
    TrackObjectControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig);
    ~TrackObjectControlLoop();
    void run(NovaProtocolCommand* cmd);
    std::vector<std::string> getLCDStatusList();

  private:
    SerialCommunication *_comm;
    NovaServo* _servo_x;
    NovaServo* _servo_y;
    PID *_pid_x;
    PID *_pid_y;
    pid_config _pid_config_x;
    pid_config _pid_config_y;
    pid_dynamic_values _pid_values_x;
    pid_dynamic_values _pid_values_y;
    void handleCommands(NovaProtocolCommand* cmd);
    void setSetpoint(uint8_t asset, int new_setpoint);
    void setupPIDcontroller(PID* pid, pid_config* config, pid_dynamic_values* values);
    void statusPublishPIDValues();
    void observe(NovaProtocolCommand* cmd);
    void actuate();
    void computeControl();
    void setPIDTuning(uint8_t asset, int p_value, int i_value, int d_value);
};

#endif
