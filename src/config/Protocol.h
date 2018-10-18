#ifndef Nova_Protocol_Nodes_h
#define Nova_Protocol_Nodes_h

#include <map>
#include <vector>

class NovaCommand {
  public:
    String module;
    String asset;
    String operation;
    std::vector<int> args;
};

class ProtocolNode {
  public:
    ProtocolNode() {}
    ProtocolNode(String id, int code) { _id = id; _code = code;}
    String _id;
    int _code;
    std::map<String, ProtocolNode> children;
};

class ProtocolLeaf : public ProtocolNode {
  public:
    ProtocolLeaf(String id, int code) : ProtocolNode(id, code) {}
};

class ModuleNode : public ProtocolNode {
  public:
    ModuleNode(String id, int code);
};

class ServoNode : public ProtocolNode {
  public:
    ServoNode(String id, int code);
    //std::map<String, ProtocolNode> children;
};

class UltraSoundNode : public ProtocolNode {
  public:
    UltraSoundNode(String id, int code);
    //std::map<String, ProtocolNode> children;
};

class PIDNode : public ProtocolNode {
  public:
    PIDNode(String id, int code);
    //std::map<String, ProtocolNode> children;
};

class NovaNode : public ProtocolNode {
  public:
    NovaNode(String id, int code);
    //std::map<String, ProtocolNode> children;
};

class ExternalInputNode : public ProtocolNode {
  public:
    ExternalInputNode(String id, int code);
    //std::map<String, ProtocolNode> children;
};

class KeepDistanceNode : public ProtocolNode {
  public:
    KeepDistanceNode(String id, int code);
    //std::map<String, ProtocolNode> children;
};

class TrackObjectNode : public ProtocolNode {
  public:
    TrackObjectNode(String id, int code);
    std::map<String, ProtocolNode> children;
};

class Root : public ProtocolNode {
  public:
    Root();
    std::map<String, ProtocolNode> children;
};

#endif
