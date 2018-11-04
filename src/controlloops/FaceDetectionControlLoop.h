#ifndef ControlLoop_Face_Detection_h
#define ControlLoop_Face_Detection_h

#include <config/HardwareConfig.h>
#include <config/NovaConfig.h>
#include <config/NovaConstants.h>
#include <PID_v1.h>
#include <controlloops/AbstractControlLoop.h>
#include <ProtocolLogic.hpp>

class FaceDetectionControlLoop : public AbstractControlLoop {
  public:
    FaceDetectionControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig);
    ~FaceDetectionControlLoop();
    void run(NovaProtocolCommand* cmd);
    std::string getLCDStatusString();

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
