#ifndef Nova_Constants_h
#define Nova_Constants_h

namespace NovaConstants {
  // command protocol markers
  static const char CMD_SEPARATOR = ':';
  static const char CMD_START_MARKER = '>';
  static const char CMD_END_MARKER = '<';

  // module codes
  static const int MOD_STATUS_NOVA = 0;
  static const int MOD_JOYSTICK_CONTROL_ABOLUTE = 1;
  static const int MOD_JOYSTICK_CONTROL_RELATIVE = 2;
  static const int MOD_KEYBOARD_MOUSE_CONTROL = 3;
  static const int MOD_DISTANCE_AVOIDANCE = 4;
  static const int MOD_FACE_DETECTION = 5;

  // operation codes
  static const int OP_STATUS_RECEIVE_SERVO_1 = 1;         // (degree in arg1)
  static const int OP_STATUS_RECEIVE_SERVO_2 = 2;         // (degree in arg1)
  static const int OP_STATUS_RECEIVE_SERVO_3 = 3;         // (degree in arg1)
  static const int OP_STATUS_RECEIVE_SERVO_4 = 4;         // (degree in arg1)
  static const int OP_STATUS_RECEIVE_SERVO_5 = 5;         // (degree in arg1)
  static const int OP_STATUS_RECEIVE_USS = 6;             // (distance in arg1)
  static const int OP_STATUS_RECEIVE_JOY_LEFT = 7;        // (x, y, pushed in arg1, arg2, arg3)
  static const int OP_STATUS_RECEIVE_JOY_RIGHT = 8;        // (x, y, pushed in arg1, arg2, arg3)
  static const int OP_STATUS_SEND_SET_MODE = 9;           // (set new modulecode as arg1)

  static const int OP_DISTANCE_SET_MIN_DIST = 1;          // (new minimum distance in arg1)
  static const int OP_DISTANCE_SET_MAX_DIST = 2;          // (new maximum distance in arg1)
  static const int OP_DISTANCE_SET_SETPOINT = 3;          // (new setpoint in arg1)
  static const int OP_DISTANCE_SET_PID_TUNING = 4;        // (p, i, d in arg1, arg2, arg3)

  static const int OP_FACE_DETECTION_X_SETPOINT = 1;      // (new setpoint in arg1)
  static const int OP_FACE_DETECTION_Y_SETPOINT = 2;      // (new setpoint in arg1)
  static const int OP_FACE_DETECTION_X_PID_TUNING = 3;    // (p, i, d in arg1, arg2, arg3)
  static const int OP_FACE_DETECTION_Y_PID_TUNING = 4;    // (p, i, d in arg1, arg2, arg3)
  static const int OP_FACE_DETECTION_SET_COORDINATES = 5; // (x, y) in arg1, arg2)
  static const int OP_FACE_DETECTION_ACK_COORDINATES = 6; // ack for received coordinates
}

#endif
