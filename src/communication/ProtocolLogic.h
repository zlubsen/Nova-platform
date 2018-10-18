#ifndef Nova_Protocol_Logic_h
#define Nova_Protocol_Logic_h

#include <config/Protocol.h>
#include <vector>

class NovaProtocolCommandBuilder {
  public:
    NovaProtocolCommandBuilder() {}
    NovaProtocolCommandBuilder* setModule(String module);
    NovaProtocolCommandBuilder* setAsset(String asset);
    NovaProtocolCommandBuilder* setOperation(String operation);
    NovaProtocolCommandBuilder* setArgs(std::vector<int> args);
    NovaProtocolCommandBuilder* setModeArg(String module_name);
    std::vector<int>* build();
    static NovaProtocolCommandBuilder* createCommand();
  private:
    ProtocolNode* _module = nullptr;
    ProtocolNode* _asset = nullptr;
    ProtocolNode* _operation = nullptr;
    std::vector<int> _args;
    ProtocolNode _root = Root();
};

class NovaProtocolCommandReader {
  public:
    NovaProtocolCommandReader();
    NovaCommand readCommand(std::vector<int8_t>* received);
  private:
    std::map<String, std::vector<String>> _lookup;
    void initLookupTree();
    void traverseModules(ProtocolNode* node);
    void traverseAssets(ProtocolNode* node, std::vector<int8_t>* code_parts, std::vector<String>* id_parts);
    void traverseOperations(ProtocolNode* node, std::vector<int8_t>* code_parts, std::vector<String>* id_parts);
    void addToLookup(std::vector<int8_t>* codes, std::vector<String>* ids);
};

#endif
