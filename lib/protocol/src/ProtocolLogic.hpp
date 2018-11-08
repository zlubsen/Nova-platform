#ifndef Nova_Protocol_Logic_h
#define Nova_Protocol_Logic_h

#include <Arduino.h>
#include "Protocol.hpp"
#include <vector>
#include <string>

class LookupEntry {
  public:
    uint8_t module;
    uint8_t asset;
    uint8_t operation;
    LookupEntry() {}
    LookupEntry(uint8_t m, uint8_t a, uint8_t o) {module = m; asset = a; operation = o;}
};

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
    void reset();
    static NovaProtocolCommandBuilder* createBuilder();
  private:
    ProtocolNode* _module = nullptr;
    ProtocolNode* _asset = nullptr;
    ProtocolNode* _operation = nullptr;
    std::vector<int> _args;
    ProtocolNode* _root;
    ProtocolNode* _dummy;
    bool _root_injected = false;
};

class NovaProtocolCommandReader {
  public:
    NovaProtocolCommandReader();
    NovaProtocolCommandReader(ProtocolNode* root);
    ~NovaProtocolCommandReader();
    void readCommand(std::vector<int>* received, NovaProtocolCommand* cmd);
    std::map<uint16_t, LookupEntry> _lookup;
  private:
    void initLookupTree(ProtocolNode* root);
    void traverseModules(ProtocolNode* node);
    void traverseAssets(ProtocolNode* node, std::vector<uint8_t>* code_parts, std::vector<uint8_t>* id_parts);
    void traverseOperations(ProtocolNode* node, std::vector<uint8_t>* code_parts, std::vector<uint8_t>* id_parts);
    void addToLookup(std::vector<uint8_t>* codes, std::vector<uint8_t>* ids);
    uint16_t createKey(uint8_t module, uint8_t asset, uint8_t operation);
};

#endif
