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
  int direction;
  int mode;
} pid_config;

typedef struct {
  double input;
  double output;
  double setpoint;
} pid_dynamic_values;

typedef struct {
  double setpoint;
  double minDistance;
  double maxDistance;
  int servoAngle;
} ultrasoundsensor_config;

class NovaConfig {
  public:
    NovaConfig();
    joystick_filter_constants _joy_left_config_absolute;
    joystick_filter_constants _joy_right_config_absolute;
    joystick_filter_constants _joy_left_config_relative;
    joystick_filter_constants _joy_right_config_relative;
    pid_config _distance_avoid_pid_config;
    ultrasoundsensor_config _ultrasound_config;
    pid_config _face_detection_pid_config_x;
    pid_config _face_detection_pid_config_y;
    int _status_publish_frequency_ms;
    int _joystick_control_delay_frequency_ms;
    int _joystick_relative_degrees_range;
    int _lcd_menu_timeout_ms;
    int _lcd_menu_background_color;
};

#endif
