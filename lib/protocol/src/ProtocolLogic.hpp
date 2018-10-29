#ifndef Nova_Protocol_Logic_h
#define Nova_Protocol_Logic_h

#include <Arduino.h>
#include "Protocol.hpp"
#include <vector>
#include <string>

class NovaProtocolCommandBuilder {
  public:
    NovaProtocolCommandBuilder();
    NovaProtocolCommandBuilder(ProtocolNode* root);
    ~NovaProtocolCommandBuilder();
    NovaProtocolCommandBuilder* setModule(uint8_t module);
    NovaProtocolCommandBuilder* setAsset(uint8_t asset);
    NovaProtocolCommandBuilder* setOperation(uint8_t operation);
    NovaProtocolCommandBuilder* setArgs(std::vector<int> args);
    NovaProtocolCommandBuilder* setModeArg(uint8_t module_name);
    std::vector<int>* build();
    static NovaProtocolCommandBuilder* createCommand();
  //private:
    ProtocolNode* _module = nullptr;
    ProtocolNode* _asset = nullptr;
    ProtocolNode* _operation = nullptr;
    std::vector<int> _args;
    ProtocolNode* _root;
    ProtocolNode* _dummy;
  private:
    bool _root_injected = false;
};

class NovaProtocolCommandReader {
  public:
    NovaProtocolCommandReader();
    NovaProtocolCommand* readCommand(std::vector<int>* received);
  private:
    std::map<std::string, std::vector<uint8_t>> _lookup;
    void initLookupTree();
    void traverseModules(ProtocolNode* node);
    void traverseAssets(ProtocolNode* node, std::vector<uint8_t>* code_parts, std::vector<uint8_t>* id_parts);
    void traverseOperations(ProtocolNode* node, std::vector<uint8_t>* code_parts, std::vector<uint8_t>* id_parts);
    void addToLookup(std::vector<uint8_t>* codes, std::vector<uint8_t>* ids);
};

#endif
