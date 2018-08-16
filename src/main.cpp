#include <Arduino.h>
#include <communication/SerialCommunication.h>
#include <config/HardwareConfig.h>
#include <config/NovaConfig.h>
#include <config/NovaConstants.h>
#include <ControlLoop/JoystickControlLoop.h>
#include <ControlLoop/DistanceAvoidControlLoop.h>
#include <ControlLoop/FaceDetectionControlLoop.h>

HardwareConfig* hardwareConfig;
NovaConfig* novaConfig;

SerialCommunication* comm;

//AbstractControlLoop* control_loops[2];

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

  //control_loops[0] = faceDetectionControlLoop;
  //control_loops[1] = ...
}

void loop() {
  comm->run();

  NovaCommand *cmd = comm->readCommand(); // now processes one command per loop

  //for(loop : control_loops) {
  //  loop->run(cmd);
  //}
  //joyControlLoop->run();
  //distanceAvoidControlLoop->run();
  faceDetectionControlLoop->run(cmd);
}
