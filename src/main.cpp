#include <Arduino.h>
#include <communication/SerialCommunication.hpp>
#include <config/HardwareConfig.h>
#include <config/NovaConfig.h>
#include <config/NovaConstants.h>
#include <controlloops/AbstractControlLoop.hpp>
#include <controlloops/ModeSelectControlLoop.hpp>
#include <controlloops/StatusPublishLoop.hpp>
#include <controlloops/JoystickAbsoluteControlLoop.hpp>
#include <controlloops/JoystickRelativeControlLoop.hpp>
#include <controlloops/ExternalInputControlLoop.hpp>
#include <controlloops/KeepDistanceControlLoop.hpp>
#include <controlloops/TrackObjectControlLoop.hpp>

#include <vector>

HardwareConfig* hardwareConfig;
NovaConfig* novaConfig;

SerialCommunication* comm;

std::vector<AbstractControlLoop*>* controlLoops;

void setup() {
  hardwareConfig = new HardwareConfig();
  novaConfig = new NovaConfig();

  comm = hardwareConfig->comm;

  ModeSelectControlLoop* modeSelectControlLoop = new ModeSelectControlLoop(hardwareConfig, novaConfig);
  controlLoops = modeSelectControlLoop->getActiveControlLoops();

  hardwareConfig->activateServos();
}

void loop() {
  comm->run();

  NovaProtocolCommand *cmd = comm->readCommand();

  for(AbstractControlLoop* loop : *controlLoops) {
    loop->run(cmd);
  }

  delete cmd;
}
