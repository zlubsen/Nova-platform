 #ifndef Nova_Protocol_Nodes_h
#define Nova_Protocol_Nodes_h

#include <map>
#include <vector>
#include <string>

#define cmd_nova 1
#define cmd_joystick_absolute 2
#define cmd_joystick_relative 3
#define cmd_external_input 4
#define cmd_keep_distance 5
#define cmd_track_object 6

#define cmd_servo1 7
#define cmd_servo2 8
#define cmd_servo3 9
#define cmd_servo4 10
#define cmd_servo5 11
#define cmd_ultrasound 12
#define cmd_set_mode 13

#define cmd_get_degree 14
#define cmd_set_degree 15
#define cmd_set_degree_steps 16

#define cmd_get_distance 17

#define cmd_toggle_auto 18
#define cmd_set_setpoint 19
#define cmd_set_tuning 20
#define cmd_get_tuning 21

#define cmd_set_min_distance 22
#define cmd_set_max_distance 23

#define cmd_set_coordinates 24
#define cmd_ack_coordinates 25

#define cmd_pid 26
#define cmd_pid_x 27
#define cmd_pid_y 28

class NovaProtocolCommand {
  public:
    NovaProtocolCommand() {}
    uint8_t module;
    uint8_t asset;
    uint8_t operation;
    std::vector<int> args;
};

class ProtocolNode {
  public:
    ProtocolNode() {}
    ProtocolNode(uint8_t id, uint8_t code) { _id = id; _code = code;}
    uint8_t _id;
    uint8_t _code;
    std::map<uint8_t, ProtocolNode> children;
    virtual bool isLeaf() { return false; }
};

class ProtocolLeaf : public ProtocolNode {
  public:
    ProtocolLeaf(uint8_t id, uint8_t code) : ProtocolNode(id, code) {}
    bool isLeaf() { return true; }
    std::map<uint8_t, ProtocolNode> children;
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
