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
    joystick_filter_constants _joy_left_config;
    joystick_filter_constants _joy_right_config;
    pid_config _distance_avoid_pid_config;
    ultrasoundsensor_config _ultrasound_config;
    pid_config _face_detection_pid_config_x;
    pid_config _face_detection_pid_config_y;

    // module codes
    int MOD_STATUS_NOVA = 0;
    int MOD_JOYSTICK_CONTROL_ABOLUTE = 1;
    int MOD_JOYSTICK_CONTROL_RELATIVE = 2;
    int MOD_KEYBOARD_MOUSE_CONTROL = 3;
    int MOD_DISTANCE_AVOIDANCE = 4;
    int MOD_FACE_DETECTION = 5;

    // operation codes
    int OP_STATUS_RECEIVE_SERVO_1 = 1;         // (degree in arg1)
    int OP_STATUS_RECEIVE_SERVO_2 = 2;         // (degree in arg1)
    int OP_STATUS_RECEIVE_SERVO_3 = 3;         // (degree in arg1)
    int OP_STATUS_RECEIVE_SERVO_4 = 4;         // (degree in arg1)
    int OP_STATUS_RECEIVE_SERVO_5 = 5;         // (degree in arg1)
    int OP_STATUS_RECEIVE_USS = 6;             // (distance in arg1)
    int OP_STATUS_RECEIVE_JOY_LEFT = 7;        // (x, y, pushed in arg1, arg2, arg3)
    int OP_STATUS_RECEIVE_JOY_RIGHT = 8;        // (x, y, pushed in arg1, arg2, arg3)
    int OP_STATUS_SEND_SET_MODE = 9;           // (set new modulecode as arg1)

    int OP_DISTANCE_SET_MIN_DIST = 1;          // (new minimum distance in arg1)
    int OP_DISTANCE_SET_MAX_DIST = 2;          // (new maximum distance in arg1)
    int OP_DISTANCE_SET_SETPOINT = 3;          // (new setpoint in arg1)
    int OP_DISTANCE_SET_PID_TUNING = 4;        // (p, i, d in arg1, arg2, arg3)

    int OP_FACE_DETECTION_X_SETPOINT = 1;      // (new setpoint in arg1)
    int OP_FACE_DETECTION_Y_SETPOINT = 2;      // (new setpoint in arg1)
    int OP_FACE_DETECTION_X_PID_TUNING = 3;    // (p, i, d in arg1, arg2, arg3)
    int OP_FACE_DETECTION_Y_PID_TUNING = 4;    // (p, i, d in arg1, arg2, arg3)
    int OP_FACE_DETECTION_SET_COORDINATES = 5; // (x, y) in arg1, arg2)
    int OP_FACE_DETECTION_ACK_COORDINATES = 6; // ack for received coordinates
};

#endif
