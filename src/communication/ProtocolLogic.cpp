#include <Arduino.h>
#include "ProtocolLogic.h"

NovaProtocolCommandBuilder* NovaProtocolCommandBuilder::setModule(String module) {
  if(_root.children.count(module))
    _module = &(_root.children[module]);

  return this;
}

NovaProtocolCommandBuilder* NovaProtocolCommandBuilder::setAsset(String asset) {
  if(_module != nullptr && _root.children.count(asset))
      _asset = &(_root.children[asset]);

  return this;
}

NovaProtocolCommandBuilder* NovaProtocolCommandBuilder::setOperation(String operation) {
  if(_module != nullptr) {
    if(_module->children.count(operation)) {
      _operation = &(_module->children[operation]);
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

NovaProtocolCommandBuilder* NovaProtocolCommandBuilder::setModeArg(String module_name) {
  _args.push_back(_root.children[module_name]._code);

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
  } else return nullptr;
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
  for(String key : node->children) {
    ProtocolNode* n = node->children[key];
    if(n->children.size() > 0) {
      traverseAssets(n, , int *id_parts)
    } else {

    }
  }
}

void NovaProtocolCommandReader::traverseAssets(ProtocolNode* node, std::vector<int8_t>* code_parts, std::vector<String>* id_parts) {

}

void NovaProtocolCommandReader::traverseOperations(ProtocolNode* node, std::vector<int8_t>* code_parts, std::vector<String>* id_parts) {

}

void NovaProtocolCommandReader::addToLookup(std::vector<int8_t>* codes, std::vector<String>* ids) {
  String key = "";

  _lookup[key] = ids;
}

NovaCommand NovaProtocolCommandReader::readCommand(std::vector<int8_t>* received) {

}
