#ifndef Nova_Protocol_h
#define Nova_Protocol_h

#include <map>

class ProtocolNode {
  public:
    ProtocolNode(String id, int code);
    String _id;
    int _code;
};

class ProtocolLeaf {
  public:
    ProtocolLeaf(String id, int code);
    String _id;
    int _code;
};

class ModuleNode : public ProtocolNode {
  public:
    ModuleNode();
};

class ServoNode : public ProtocolNode {
  public:
    ServoNode(String id, int code);
    std::map<String, int> children;
};

class UltraSoundNode : public ProtocolNode {
  public:
    UltraSoundNode(String id, int code);
    std::map<String, int> children;
};

class PIDNode : public ProtocolNode {
  public:
    PIDNode(String id, int code);
    std::map<String, int> children;
};

class NovaNode : public ProtocolNode {
  public:
    NovaNode(String id, int code);
    std::map<String, int> children;
};

class ExternalInputNode : public ProtocolNode {
  public:
    ExternalInputNode(String id, int code);
    std::map<String, int> children;
};

class KeepDistanceNode : public ProtocolNode {
  public:
    ExternalInputNode(String id, int code);
    std::map<String, int> children;
}

class TrackObjectNode : public ProtocolNode {
  public:
    TrackObjectNode(String id, int code);
    std::map<String, int> children;
}

class Root : public ProtocolNode {
  public:
    Root();
    std::map<String, int> children;
}

class NovaProtocolCommandBuilder {

};

class NovaProtocolCommandReader {

};

#endif
