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
} pid_config;

class NovaConfig {
  public:
    NovaConfig();
    joystick_filter_constants _joy_left_config;
    joystick_filter_constants _joy_right_config;
};

#endif
