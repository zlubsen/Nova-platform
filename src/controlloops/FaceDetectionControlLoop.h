#ifndef ControlLoop_Face_Detection_h
#define ControlLoop_Face_Detection_h

#include <config/HardwareConfig.h>
#include <config/NovaConfig.h>
#include <config/NovaConstants.h>
#include <PID_v1.h>
#include <controlloops/AbstractControlLoop.h>

class FaceDetectionControlLoop : public AbstractControlLoop {
  public:
    FaceDetectionControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig);
    void run(NovaCommand* cmd);
    std::string getLCDStatusString();

  private:
    NovaServo* _servo_x;
    NovaServo* _servo_y;
    PID *_pid_x;
    PID *_pid_y;
    pid_config _pid_config_x;
    pid_config _pid_config_y;
    pid_dynamic_values _pid_values_x;
    pid_dynamic_values _pid_values_y;
    SerialCommunication *_comm;
    void handleCommands(NovaCommand* cmd);
    void setSetpoint(int opcode, int new_setpoint);
    void setupPIDcontroller(PID* pid, pid_config* config, pid_dynamic_values* values);
    void statusPublishPIDValues();
    void observe(NovaCommand* cmd);
    void actuate();
    void computeControl();
    void setPIDTuning(int opcode, int p_value, int i_value, int d_value);
};

#endif
