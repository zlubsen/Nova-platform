#include <Arduino.h>
#include <SerialCommunication.h>
#include "ControlLoop/JoystickControlLoop.h"
#include "ControlLoop/DistanceAvoidControlLoop.h"
#include "ControlLoop/FaceDetectionControlLoop.h"

HardwareConfig* hardwareConfig;
NovaConfig* novaConfig;

SerialCommunication* comm;

JoystickControlLoop* joyControlLoop;
DistanceAvoidControlLoop* distanceAvoidControlLoop;
FaceDetectionControlLoop* faceDetectionControlLoop;

joystick_status left;
joystick_status right;

void setup() {
  hardwareConfig = new HardwareConfig();
  novaConfig = new NovaConfig();

  comm = hardwareConfig->comm;

  joyControlLoop = new JoystickControlLoop(hardwareConfig, novaConfig);
  distanceAvoidControlLoop = new DistanceAvoidControlLoop(hardwareConfig, novaConfig);
  faceDetectionControlLoop = new FaceDetectionControlLoop(hardwareConfig, novaConfig);
}

void loop() {
  comm->run();

  NovaCommand *cmd = comm->readCommand();

  //joyControlLoop->run();
  //distanceAvoidControlLoop->run();
  faceDetectionControlLoop->run(cmd);
}
