#include "NovaConfig.h"

NovaConfig::NovaConfig() {
  _joy_left_config.x = 0.05;
  _joy_left_config.y = 0.05;
  _joy_right_config.x = 0.08;
  _joy_right_config.y = 0.08;

  _ultrasound_pid_config.Kp = 0.51;
  _ultrasound_pid_config.Ki = 1.1;
  _ultrasound_pid_config.Kd = 0.0;
  _ultrasound_pid_config.sampleTime = 10;
  _ultrasound_pid_config.outputLimitMin = -1;
  _ultrasound_pid_config.outputLimitMax = 1;

  _ultrasound_config.setpoint = 15;
  _ultrasound_config.minDistance = 9;
  _ultrasound_config.maxDistance = 30;
  _ultrasound_config.servoAngle = 110;
}
