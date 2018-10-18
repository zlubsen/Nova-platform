#include <Arduino.h>
#include "ProtocolLogic.h"

NovaProtocolCommandBuilder::NovaProtocolCommandBuilder() {

}

NovaProtocolCommandBuilder NovaProtocolCommandBuilder::setModule(String module) {
  if(root.children.count(module))
    _module = root.children[module];

  return this;
}

NovaProtocolCommandBuilder NovaProtocolCommandBuilder::setAsset(String asset) {
  if(_module != NULL && root.children.count(asset))
      _asset = root.children[asset];

  return this;
}

NovaProtocolCommandBuilder NovaProtocolCommandBuilder::setOperation(String operation) {
  if(_module != NULL) {
    if(_module.children.count(operation)) {
      _operation = _module.children[operation];
    } else if(_asset != NULL && _asset.children.count(operation)) {
      _operation = _asset.children[operation];
    }
  }
}

NovaProtocolCommandBuilder NovaProtocolCommandBuilder::setArgs(std::vector<char> args) {

}

NovaProtocolCommandBuilder NovaProtocolCommandBuilder::setModeArg(String module_name) {

}

std::vector<int> NovaProtocolCommandBuilder::build() {

}

static NovaProtocolCommandBuilder::createCommand() {

}

NovaCommand NovaProtocolCommandReader::readCommand(std::vector<int> received) {

}
