#include <Arduino.h>
#include <communication/SerialCommunication.h>
#include <config/HardwareConfig.h>
#include <config/NovaConfig.h>
#include <config/NovaConstants.h>
#include <controlloops/AbstractControlLoop.h>
#include <controlloops/ModeSelectControlLoop.h>
#include <controlloops/StatusPublishLoop.h>
#include <controlloops/JoystickAbsoluteControlLoop.h>
#include <controlloops/JoystickRelativeControlLoop.h>
#include <controlloops/ExternalInputControlLoop.h>
#include <controlloops/DistanceAvoidControlLoop.h>
#include <controlloops/FaceDetectionControlLoop.h>

#include <vector>

void updateControlLoops();

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
