#ifndef Nova_Protocol_Logic_h
#define Nova_Protocol_Logic_h

#include <config/Protocol.h>
#include <vector>

class NovaProtocolCommandBuilder {
  public:
    NovaProtocolCommandBuilder() {}
    NovaProtocolCommandBuilder setModule(String module);
    NovaProtocolCommandBuilder setAsset(String asset);
    NovaProtocolCommandBuilder setOperation(String operation);
    NovaProtocolCommandBuilder setArgs(std::vector<char> args);
    NovaProtocolCommandBuilder setModeArg(String module_name);
    std::vector<int> build();
    static NovaProtocolCommandBuilder createCommand();
  private:
    ProtocolNode _module = NULL;
    ProtocolNode _asset = NULL;
    ProtocolNode _operation = NULL;
    std::vector<char> _args;
    ProtocolNode _root = Root();
};

class NovaProtocolCommandReader {
  public:
    NovaCommand readCommand(std::vector<int> received);
};

#endif
