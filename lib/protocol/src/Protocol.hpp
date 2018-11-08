 #ifndef Nova_Protocol_Nodes_h
#define Nova_Protocol_Nodes_h

#include <map>
#include <vector>

// modules
#define cmd_module 0
#define cmd_nova 1
#define cmd_joystick_absolute 2
#define cmd_joystick_relative 3
#define cmd_external_input 4
#define cmd_keep_distance 5
#define cmd_track_object 6

// generic assets
#define cmd_servo1 11
#define cmd_servo2 12
#define cmd_servo3 13
#define cmd_servo4 14
#define cmd_servo5 15
#define cmd_ultrasound 16

// nova operations
#define cmd_set_mode 101
#define cmd_get_freememory 102

// keep_distance operations
#define cmd_set_min_distance 51
#define cmd_set_max_distance 52

// track_object operations
#define cmd_set_coordinates 61
#define cmd_ack_coordinates 62

// servo operations
#define cmd_get_degree 111
#define cmd_set_degree 112
#define cmd_set_degree_steps 113

// ultrasoundsensor operations
#define cmd_get_distance 121

// PID operations
#define cmd_toggle_auto 131
#define cmd_set_setpoint 132
#define cmd_set_tuning 133
#define cmd_get_tuning 134

// PID assets
#define cmd_pid 201
#define cmd_pid_x 202
#define cmd_pid_y 203

class NovaProtocolCommand {
  public:
    NovaProtocolCommand() {}
    int module;
    int asset;
    int operation;
    std::vector<int> args;
};

class ProtocolNode {
  public:
    ProtocolNode() {}
    ProtocolNode(uint8_t id, uint8_t code) { _id = id; _code = code;}
    uint8_t _id;
    uint8_t _code;
    std::map<uint8_t, ProtocolNode> children;
};

class ProtocolLeaf : public ProtocolNode {
  public:
    ProtocolLeaf(uint8_t id, uint8_t code) : ProtocolNode(id, code) {}
    const std::map<uint8_t, ProtocolNode> children;
};

class ServoNode : public ProtocolNode {
  public:
    ServoNode(uint8_t id, uint8_t code);
};

class UltraSoundNode : public ProtocolNode {
  public:
    UltraSoundNode(uint8_t id, uint8_t code);
};

class PIDNode : public ProtocolNode {
  public:
    PIDNode(uint8_t id, uint8_t code);
};

class NovaNode : public ProtocolNode {
  public:
    NovaNode(uint8_t id, uint8_t code);
};

class ModuleNode : public ProtocolNode {
  public:
    ModuleNode();
};

class ExternalInputNode : public ProtocolNode {
  public:
    ExternalInputNode(uint8_t id, uint8_t code);
};

class KeepDistanceNode : public ProtocolNode {
  public:
    KeepDistanceNode(uint8_t id, uint8_t code);
};

class TrackObjectNode : public ProtocolNode {
  public:
    TrackObjectNode(uint8_t id, uint8_t code);
};

class Root : public ProtocolNode {
  public:
    Root();
};

#endif
