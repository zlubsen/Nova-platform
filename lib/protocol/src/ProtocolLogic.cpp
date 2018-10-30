#include "ProtocolLogic.hpp"
#include <typeinfo>

NovaProtocolCommandBuilder::NovaProtocolCommandBuilder() {
  _root = new Root();
  _dummy = new ModuleNode();
}

NovaProtocolCommandBuilder::NovaProtocolCommandBuilder(ProtocolNode* root) {
  _root = root;
  _root_injected = true;
  _dummy = new ModuleNode();
}

NovaProtocolCommandBuilder::~NovaProtocolCommandBuilder() {
  if(!_root_injected)
    delete _root;

  delete _dummy;
}

NovaProtocolCommandBuilder* NovaProtocolCommandBuilder::setModule(uint8_t module) {
  if(_root->children.count(module))
    _module = &(_root->children[module]);

  return this;
}

NovaProtocolCommandBuilder* NovaProtocolCommandBuilder::setAsset(uint8_t asset) {
  if(_module != nullptr && _module->children.count(asset))
      _asset = &(_module->children[asset]);

  return this;
}

NovaProtocolCommandBuilder* NovaProtocolCommandBuilder::setOperation(uint8_t operation) {
  if(_module != nullptr) {
    if(_module->children.count(operation)) {
      _operation = &(_module->children[operation]);
      _asset = _dummy;
    } else if(_asset != nullptr && _asset->children.count(operation)) {
      _operation = &(_asset->children[operation]);
    }
  }

  return this;
}

NovaProtocolCommandBuilder* NovaProtocolCommandBuilder::setArgs(std::vector<int> args) {
  for(int arg : args) {
    _args.push_back(arg);
  }

  return this;
}

NovaProtocolCommandBuilder* NovaProtocolCommandBuilder::setModeArg(uint8_t module_name) {
  _args.push_back(_root->children[module_name]._code);

  return this;
}

std::vector<int>* NovaProtocolCommandBuilder::build() {
  if(_module != nullptr && _asset != nullptr && _operation != nullptr) {
    std::vector<int>* cmd = new std::vector<int>();

    cmd->push_back(_module->_code);
    cmd->push_back(_asset->_code);
    cmd->push_back(_operation->_code);
    cmd->push_back(_args.size());
    for(int arg : _args)
      cmd->push_back(arg);

    return cmd;
  } else {
    return nullptr;
  }
}

NovaProtocolCommandBuilder* NovaProtocolCommandBuilder::createCommand() {
  NovaProtocolCommandBuilder* builder = new NovaProtocolCommandBuilder();
  return builder;
}

NovaProtocolCommandReader::NovaProtocolCommandReader() {
  initLookupTree();
}

void NovaProtocolCommandReader::initLookupTree() {
  traverseModules(new Root());
}

void NovaProtocolCommandReader::traverseModules(ProtocolNode* node) {
  std::vector<uint8_t> code_parts;
  std::vector<uint8_t> id_parts;

  for(auto const &item : node->children) {
    auto n = item.second;
    code_parts.push_back(n._code);
    id_parts.push_back(n._id);

    if(n.children.size() > 0) {
      traverseAssets(&n, &code_parts, &id_parts);
    } else {
      code_parts.push_back(0);
      code_parts.push_back(0);
      id_parts.push_back(0);
      id_parts.push_back(0);
      addToLookup(&code_parts, &id_parts);
    }

    code_parts.clear();
    id_parts.clear();
  }
}

void NovaProtocolCommandReader::traverseAssets(ProtocolNode* node, std::vector<uint8_t>* code_parts, std::vector<uint8_t>* id_parts) {
  for(auto const &item : node->children) {
    auto n = item.second;
    if(n.children.size() > 0) {
      code_parts->push_back(n._code);
      id_parts->push_back(n._id);
      traverseOperations(&n, code_parts, id_parts);
      code_parts->pop_back();
      id_parts->pop_back();
    } else {
      code_parts->push_back(0);
      code_parts->push_back(n._code);
      id_parts->push_back(0);
      id_parts->push_back(n._id);
      addToLookup(code_parts, id_parts);
      code_parts->pop_back();
      code_parts->pop_back();
      id_parts->pop_back();
      id_parts->pop_back();
    }
  }
}

void NovaProtocolCommandReader::traverseOperations(ProtocolNode* node, std::vector<uint8_t>* code_parts, std::vector<uint8_t>* id_parts) {
  for(auto const &item : node->children) {
    auto n = item.second;
    if(n.children.size() == 0) {
      code_parts->push_back(n._code);
      id_parts->push_back(n._id);
      addToLookup(code_parts, id_parts);
      code_parts->pop_back();
      id_parts->pop_back();
    }
  }
}

void NovaProtocolCommandReader::addToLookup(std::vector<uint8_t>* codes, std::vector<uint8_t>* ids) {
  uint16_t key = createKey(codes->at(0), codes->at(1), codes->at(2));

  _lookup[key] = *ids;
}

NovaProtocolCommand* NovaProtocolCommandReader::readCommand(std::vector<int>* received) {
  uint16_t key = createKey(received->at(0), received->at(1), received->at(2));

  auto parts = _lookup[key];
  NovaProtocolCommand *cmd = new NovaProtocolCommand;
  cmd->module = parts[0];
  cmd->asset = parts[1];
  cmd->operation = parts[2];

  int8_t argcount = received->at(3);
  for(int i = 0; i < argcount; i++) {
    cmd->args.push_back(received->at(i+4));
  }

  return cmd;
}

uint16_t NovaProtocolCommandReader::createKey(uint8_t module, uint8_t asset, uint8_t operation) {
  uint16_t key = 0;
  key += module * 10000;
  key += asset * 100;
  key += operation;

  return key;
}
