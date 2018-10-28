#include "Protocol.hpp"
//#include <Arduino.h>

/*ProtocolNode servo1 = ServoNode("servo1", 1);
ProtocolNode servo2 = ServoNode("servo2", 2);
ProtocolNode servo3 = ServoNode("servo3", 3);
ProtocolNode servo4 = ServoNode("servo4", 4);
ProtocolNode servo5 = ServoNode("servo5", 5);*/

Root::Root() : ProtocolNode("root", 0) {
  children["nova"] = NovaNode("nova", 0);
  children["joystick_absolute"] = ProtocolLeaf("joystick_absolute", 1);
  children["joystick_relative"] = ProtocolLeaf("joystick_relative", 2);
  children["external_input"] = ExternalInputNode("external_input", 3);
  children["keep_distance"] = KeepDistanceNode("keep_distance", 4);
  children["track_object"] = TrackObjectNode("track_object", 5);
}

/*ModuleNode::ModuleNode(std::string id, int8_t code) : ProtocolNode(id, code) {
  children["set_mode"] = ProtocolLeaf("set_mode", 1);
  children["servo1"] = ServoNode("servo1", 1);
  children["servo2"] = ServoNode("servo2", 2);
  children["servo3"] = ServoNode("servo3", 3);
  children["servo4"] = ServoNode("servo4", 4);
  children["servo5"] = ServoNode("servo5", 5);
  children["ultrasound"] = UltraSoundNode("ultrasound", 6);
}*/

ServoNode::ServoNode(std::string id, int8_t code) : ProtocolNode(id, code) {
  children["get_degree"] = ProtocolLeaf("get_degree", 1);
  children["set_degree"] = ProtocolLeaf("set_degree", 2);
  children["set_degree_steps"] = ProtocolLeaf("set_degree_steps", 3);
}

UltraSoundNode::UltraSoundNode(std::string id, int8_t code) : ProtocolNode(id, code) {
  children["get_distance"] = ProtocolLeaf("get_distance", 1);
}

PIDNode::PIDNode(std::string id, int8_t code) : ProtocolNode(id, code) {
  children["toggle_auto"] = ProtocolLeaf("toggle_auto", 1);
  children["set_setpoint"] = ProtocolLeaf("set_setpoint", 2);
  children["set_tuning"] = ProtocolLeaf("set_tuning", 3);
  children["get_tuning"] = ProtocolLeaf("get_tuning", 4);
}

NovaNode::NovaNode(std::string id, int8_t code) : ProtocolNode(id, code) {
  children["set_mode"] = ProtocolLeaf("set_mode", 1);
  children["servo1"] = ServoNode("servo1", 1);
  children["servo2"] = ServoNode("servo2", 2);
  children["servo3"] = ServoNode("servo3", 3);
  children["servo4"] = ServoNode("servo4", 4);
  children["servo5"] = ServoNode("servo5", 5);
  children["ultrasound"] = UltraSoundNode("ultrasound", 6);
}

ExternalInputNode::ExternalInputNode(std::string id, int8_t code) : ProtocolNode(id, code) {
  children["servo1"] = ServoNode("servo1", 1);
  children["servo2"] = ServoNode("servo2", 2);
  children["servo3"] = ServoNode("servo3", 3);
  children["servo4"] = ServoNode("servo4", 4);
  children["servo5"] = ServoNode("servo5", 5);
}

KeepDistanceNode::KeepDistanceNode(std::string id, int8_t code) : ProtocolNode(id, code) {
  children["set_min_distance"] = ProtocolLeaf("set_min_distance", 1);
  children["set_max_distance"] = ProtocolLeaf("set_max_distance", 2);
  children["pid"] = PIDNode("pid", 1);
}

TrackObjectNode::TrackObjectNode(std::string id, int8_t code) : ProtocolNode(id, code) {
  children["set_coordinates"] = ProtocolLeaf("set_coordinates", 1);
  children["ack_coordinates"] = ProtocolLeaf("ack_coordinates", 2);
  children["pid_x"] = PIDNode("pid_x", 1);
  children["pid_y"] = PIDNode("pid_y", 2);
}
