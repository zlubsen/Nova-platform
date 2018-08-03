#ifndef ControlLoop_Face_Detection_h
#define ControlLoop_Face_Detection_h

#include <HardwareConfig.h>
#include <NovaConfig.h>
#include <PID_v1.h>

class FaceDetectionControlLoop {
  public:
    FaceDetectionControlLoop(HardwareConfig *hardwareConfig, NovaConfig *novaConfig);
    void run(NovaCommand* cmd);

  private:
    NovaServo* _servo_x;
    NovaServo* _servo_y;
    PID *_pid_x;
    PID *_pid_y;
    pid_config _pid_config_x;
    pid_config _pid_config_y;
    pid_dynamic_values _pid_values_x;
    pid_dynamic_values _pid_values_y;
    int _angle_x;
    int _angle_y;
    int _serialCount = 0;
    int _serialInArray[2];
    Communication *_comm;
    void setupPIDcontroller(PID* pid, pid_config* config, pid_dynamic_values* values);
    void observe(NovaCommand* cmd);
    void actuate();
    void computeControl();
};

#endif
