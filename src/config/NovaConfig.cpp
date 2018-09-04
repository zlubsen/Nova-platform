#include "NovaConfig.h"
#include <PID_v1.h>

NovaConfig::NovaConfig() {
  _status_publish_frequency_ms = 2000;
  _joystick_control_delay_frequency_ms = 30;

  _joy_left_config_absolute.x = 0.05;
  _joy_left_config_absolute.y = 0.05;
  _joy_right_config_absolute.x = 0.08;
  _joy_right_config_absolute.y = 0.08;

  _joy_left_config_relative.x = 0.2;
  _joy_left_config_relative.y = 0.2;
  _joy_right_config_relative.x = 0.4;
  _joy_right_config_relative.y = 0.4;

  _joystick_relative_degrees_range = 10;

  _distance_avoid_pid_config.Kp = 0.51;
  _distance_avoid_pid_config.Ki = 1.1;
  _distance_avoid_pid_config.Kd = 0;
  _distance_avoid_pid_config.sampleTime = 10;
  _distance_avoid_pid_config.outputLimitMin = -1;
  _distance_avoid_pid_config.outputLimitMax = 1;
  _distance_avoid_pid_config.direction = DIRECT;
  _distance_avoid_pid_config.mode = AUTOMATIC;

  _ultrasound_config.setpoint = 15;
  _ultrasound_config.minDistance = 9;
  _ultrasound_config.maxDistance = 30;
  _ultrasound_config.servoAngle = 110;

  _face_detection_pid_config_x.Kp = 0.016;//0.016;
  _face_detection_pid_config_x.Ki = 0.012;//0.012;
  _face_detection_pid_config_x.Kd = 0;
  _face_detection_pid_config_x.sampleTime = 1;
  _face_detection_pid_config_x.outputLimitMin = -35;
  _face_detection_pid_config_x.outputLimitMax = 35;
  _face_detection_pid_config_x.direction = DIRECT;
  _face_detection_pid_config_x.mode = AUTOMATIC;

  _face_detection_pid_config_y.Kp = 0.028;//0.028;
  _face_detection_pid_config_y.Ki = 0.026;//0.026;
  _face_detection_pid_config_y.Kd = 0;
  _face_detection_pid_config_y.sampleTime = 1;
  _face_detection_pid_config_y.outputLimitMin = -35;
  _face_detection_pid_config_y.outputLimitMax = 35;
  _face_detection_pid_config_y.direction = DIRECT;
  _face_detection_pid_config_y.mode = AUTOMATIC;
}
