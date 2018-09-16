#include <Arduino.h>
#include "ModeSelectControlLoop.h"

ModeSelectControlLoop::ModeSelectControlLoop(HardwareConfig*  hardwareConfig, NovaConfig* novaConfig) {

}

void ModeSelectControlLoop::run(NovaCommand* cmd) {
  if(cmd != NULL && cmd->modulecode == NovaConstants::MOD_STATUS_NOVA) {
    //handleCommands(cmd);
  }

}
