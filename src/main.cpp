#include <Arduino.h>
#include <communication/SerialCommunication.h>
#include <config/HardwareConfig.h>
#include <config/NovaConfig.h>
#include <config/NovaConstants.h>
#include <controlloops/AbstractControlLoop.h>
#include <controlloops/StatusPublishLoop.h>
#include <controlloops/JoystickControlLoop.h>
#include <controlloops/DistanceAvoidControlLoop.h>
#include <controlloops/FaceDetectionControlLoop.h>

HardwareConfig* hardwareConfig;
NovaConfig* novaConfig;

SerialCommunication* comm;

//AbstractControlLoop* controlLoops[2];

StatusPublishLoop* statusPublishLoop;

JoystickControlLoop* joyControlLoop;
DistanceAvoidControlLoop* distanceAvoidControlLoop;
FaceDetectionControlLoop* faceDetectionControlLoop;

joystick_status left;
joystick_status right;

void setup() {
  hardwareConfig = new HardwareConfig();
  novaConfig = new NovaConfig();

  comm = hardwareConfig->comm;

  statusPublishLoop = new StatusPublishLoop(hardwareConfig, novaConfig->_status_publish_frequency_ms);
  joyControlLoop = new JoystickControlLoop(hardwareConfig, novaConfig);
  distanceAvoidControlLoop = new DistanceAvoidControlLoop(hardwareConfig, novaConfig);
  faceDetectionControlLoop = new FaceDetectionControlLoop(hardwareConfig, novaConfig);

  //controlLoops[0] = statusPublishLoop;
  //controlLoops[1] = faceDetectionControlLoop
}

void loop() {
  comm->run();

  NovaCommand *cmd = comm->readCommand(); // now processes one command per loop

  //for(loop : controlLoops) {
  //  loop->run(cmd);
  //}
  //joyControlLoop->run();
  //distanceAvoidControlLoop->run();
  faceDetectionControlLoop->run(cmd);
  statusPublishLoop->run(cmd);
}
