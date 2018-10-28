#ifndef Nova_Protocol_Nodes_h
#define Nova_Protocol_Nodes_h

#include <map>
#include <vector>
#include <string>

const std::string cmdstr_nova = "nova";
const std::string cmdstr_joystick_absolute = "joystick_absolute";
const std::string cmdstr_joystick_relative = "joystick_relative";
const std::string cmdstr_external_input = "external_input";
const std::string cmdstr_keep_distance = "keep_distance";
const std::string cmdstr_track_object = "track_object";

const std::string cmdstr_servo = "servo";
const std::string cmdstr_ultrasound = "ultrasound";
const std::string cmdstr_set_mode = "set_mode";

const std::string cmdstr_get_degree = "get_degree";
const std::string cmdstr_set_degree = "set_degree";
const std::string cmdstr_set_degree_steps = "set_degree_steps";

const std::string cmdstr_get_distance = "get_distance";

const std::string cmdstr_toggle_auto = "toggle_auto";
const std::string cmdstr_set_setpoint = "set_setpoint";
const std::string cmdstr_set_tuning = "set_tuning";
const std::string cmdstr_get_tuning = "get_tuning";

const std::string cmdstr_set_min_distance = "set_min_distance";
const std::string cmdstr_set_max_distance = "set_max_distance";

const std::string cmdstr_set_coordinates = "set_coordinates";
const std::string cmdstr_ack_coordinates = "ack_coordinates";

class NovaProtocolCommand {
  public:
    NovaProtocolCommand() {}
    std::string module;
    std::string asset;
    std::string operation;
    std::vector<int> args;
};

class ProtocolNode {
  public:
    ProtocolNode() {}
    ProtocolNode(std::string id, int8_t code) { _id = id; _code = code;}
    std::string _id;
    int8_t _code;
    std::map<std::string, ProtocolNode> children;
    bool isLeaf() {return false;}
};

class ProtocolLeaf : public ProtocolNode {
  public:
    ProtocolLeaf(std::string id, int8_t code) : ProtocolNode(id, code) {}
    bool isLeaf() {return true;}
};

class ModuleNode : public ProtocolNode {
  public:
    ModuleNode(std::string id, int8_t code);
};

class ServoNode : public ProtocolNode {
  public:
    ServoNode(std::string id, int8_t code);
};

class UltraSoundNode : public ProtocolNode {
  public:
    UltraSoundNode(std::string id, int8_t code);
};

class PIDNode : public ProtocolNode {
  public:
    PIDNode(std::string id, int8_t code);
};

class NovaNode : public ProtocolNode {
  public:
    NovaNode(std::string id, int8_t code);
};

class ExternalInputNode : public ProtocolNode {
  public:
    ExternalInputNode(std::string id, int8_t code);
};

class KeepDistanceNode : public ProtocolNode {
  public:
    KeepDistanceNode(std::string id, int8_t code);
};

class TrackObjectNode : public ProtocolNode {
  public:
    TrackObjectNode(std::string id, int8_t code);
};

class Root : public ProtocolNode {
  public:
    Root();
};

#endif
