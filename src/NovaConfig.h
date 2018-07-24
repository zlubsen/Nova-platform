#ifndef Nova_Config_h
#define Nova_Config_h

typedef struct {
  float x;
  float y;
} joystick_filter_constants;

typedef struct {
  double Kp;
  double Ki;
  double Kd;
  int sampleTime;
  int outputLimitMin;
  int outputLimitMax;
} pid_config;

typedef struct {
  double setpoint;
  double minDistance;
  double maxDistance;
  int servoAngle;
} ultrasoundsensor_config;

class NovaConfig {
  public:
    NovaConfig();
    joystick_filter_constants _joy_left_config;
    joystick_filter_constants _joy_right_config;
    pid_config _ultrasound_pid_config;
    ultrasoundsensor_config _ultrasound_config;
};

#endif
