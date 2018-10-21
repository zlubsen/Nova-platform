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

AbstractControlLoop* controlLoops[3];
//std::vector<AbstractControlLoop*> controlLoops;

StatusPublishLoop* statusPublishLoop;
ModeSelectControlLoop* modeSelectControlLoop;

void setup() {
  hardwareConfig = new HardwareConfig();
  novaConfig = new NovaConfig();

  comm = hardwareConfig->comm;

  modeSelectControlLoop = new ModeSelectControlLoop(hardwareConfig, novaConfig);
  statusPublishLoop = modeSelectControlLoop->statusPublishLoop;

  // TODO how to have controlLoops point to an array in ModeSelectControlLoop?
  controlLoops[0] = modeSelectControlLoop;
  controlLoops[1] = statusPublishLoop;
  controlLoops[2] = modeSelectControlLoop->activeControlLoop;
  //controlLoops.reserve(3);
  //controlLoops.push_back(modeSelectControlLoop);
  //controlLoops.push_back(statusPublishLoop);
  //controlLoops.push_back(modeSelectControlLoop->activeControlLoop);

  hardwareConfig->activateServos();
}

void loop() {
  comm->run();

  NovaCommand *cmd = comm->readCommand();

  for(AbstractControlLoop* loop : controlLoops) {
    loop->run(cmd);
  }

  updateControlLoops();

  delete cmd;
}

void updateControlLoops() {
  controlLoops[2] = modeSelectControlLoop->activeControlLoop;
}
