#ifndef Nova_Protocol_Nodes_h
#define Nova_Protocol_Nodes_h

#include <map>
#include <vector>
#include <string>

class NovaCommand {
  public:
    NovaCommand() {}
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
