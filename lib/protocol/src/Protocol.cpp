#include "Protocol.hpp"
#include <Arduino.h>

Root::Root() : ProtocolNode(0, 0) {
  children[cmd_nova] = NovaNode(cmd_nova, 0);
  children[cmd_joystick_absolute] = ProtocolLeaf(cmd_joystick_absolute, 1);
  children[cmd_joystick_relative] = ProtocolLeaf(cmd_joystick_relative, 2);
  children[cmd_external_input] = ExternalInputNode(cmd_external_input, 3);
  children[cmd_keep_distance] = KeepDistanceNode(cmd_keep_distance, 4);
  children[cmd_track_object] = TrackObjectNode(cmd_track_object, 5);
}

ServoNode::ServoNode(uint8_t id, uint8_t code) : ProtocolNode(id, code) {
  children[cmd_get_degree] = ProtocolLeaf(cmd_get_degree, 1);
  children[cmd_set_degree] = ProtocolLeaf(cmd_set_degree, 2);
  children[cmd_set_degree_steps] = ProtocolLeaf(cmd_set_degree_steps, 3);
}

UltraSoundNode::UltraSoundNode(uint8_t id, uint8_t code) : ProtocolNode(id, code) {
  children[cmd_get_distance] = ProtocolLeaf(cmd_get_distance, 1);
}

PIDNode::PIDNode(uint8_t id, uint8_t code) : ProtocolNode(id, code) {
  children[cmd_toggle_auto] = ProtocolLeaf(cmd_toggle_auto, 1);
  children[cmd_set_setpoint] = ProtocolLeaf(cmd_set_setpoint, 2);
  children[cmd_set_tuning] = ProtocolLeaf(cmd_set_tuning, 3);
  children[cmd_get_tuning] = ProtocolLeaf(cmd_get_tuning, 4);
}

NovaNode::NovaNode(uint8_t id, uint8_t code) : ProtocolNode(id, code) {
  children[cmd_set_mode] = ProtocolLeaf(cmd_set_mode, 1);
  children[cmd_servo1] = ServoNode(cmd_servo1, 1);
  children[cmd_servo2] = ServoNode(cmd_servo2, 2);
  children[cmd_servo3] = ServoNode(cmd_servo3, 3);
  children[cmd_servo4] = ServoNode(cmd_servo4, 4);
  children[cmd_servo5] = ServoNode(cmd_servo5, 5);
  children[cmd_ultrasound] = UltraSoundNode(cmd_ultrasound, 6);
}

ModuleNode::ModuleNode() : ProtocolNode(0, 0) {
}

ExternalInputNode::ExternalInputNode(uint8_t id, uint8_t code) : ProtocolNode(id, code) {
  children[cmd_servo1] = ServoNode(cmd_servo1, 1);
  children[cmd_servo2] = ServoNode(cmd_servo2, 2);
  children[cmd_servo3] = ServoNode(cmd_servo3, 3);
  children[cmd_servo4] = ServoNode(cmd_servo4, 4);
  children[cmd_servo5] = ServoNode(cmd_servo5, 5);
}

KeepDistanceNode::KeepDistanceNode(uint8_t id, uint8_t code) : ProtocolNode(id, code) {
  children[cmd_set_min_distance] = ProtocolLeaf(cmd_set_min_distance, 1);
  children[cmd_set_max_distance] = ProtocolLeaf(cmd_set_max_distance, 2);
  children[cmd_pid] = PIDNode(cmd_pid, 1);
}

TrackObjectNode::TrackObjectNode(uint8_t id, uint8_t code) : ProtocolNode(id, code) {
  children[cmd_set_coordinates] = ProtocolLeaf(cmd_set_coordinates, 1);
  children[cmd_ack_coordinates] = ProtocolLeaf(cmd_ack_coordinates, 2);
  children[cmd_pid_x] = PIDNode(cmd_pid_x, 1);
  children[cmd_pid_y] = PIDNode(cmd_pid_y, 2);
}
