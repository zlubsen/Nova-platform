#include <Arduino.h>
#include <communication/SerialCommunication.h>
#include <config/HardwareConfig.h>
#include <config/NovaConfig.h>
#include <config/NovaConstants.h>
#include <controlloops/AbstractControlLoop.h>
#include <controlloops/StatusPublishLoop.h>
#include <controlloops/JoystickControlLoop.h>
#include <controlloops/JoystickRelativeControlLoop.h>
#include <controlloops/KeyboardMouseControlLoop.h>
#include <controlloops/DistanceAvoidControlLoop.h>
#include <controlloops/FaceDetectionControlLoop.h>

void handleCommands(NovaCommand*);
void setMode(int);

HardwareConfig* hardwareConfig;
NovaConfig* novaConfig;

SerialCommunication* comm;

AbstractControlLoop* controlLoops[2];

StatusPublishLoop* statusPublishLoop;

JoystickControlLoop* joyControlLoop;
JoystickRelativeControlLoop* joyRelativeControlLoop;
KeyboardMouseControlLoop* keyboardMouseControlLoop;
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
  joyRelativeControlLoop = new JoystickRelativeControlLoop(hardwareConfig, novaConfig);
  keyboardMouseControlLoop = new KeyboardMouseControlLoop(hardwareConfig, novaConfig);
  distanceAvoidControlLoop = new DistanceAvoidControlLoop(hardwareConfig, novaConfig);
  faceDetectionControlLoop = new FaceDetectionControlLoop(hardwareConfig, novaConfig);

  controlLoops[0] = statusPublishLoop;
  controlLoops[1] = faceDetectionControlLoop; //TODO default control loop is now face detection?
}

void handleCommands(NovaCommand* cmd) {
  if(cmd->operandcode == NovaConstants::OP_STATUS_SEND_SET_MODE) {
    setMode(cmd->arg1);
  }
}

void setMode(int mode) {
  switch (mode) {
    case NovaConstants::MOD_JOYSTICK_CONTROL_ABOLUTE:
      controlLoops[1] = joyControlLoop;
      break;
    case NovaConstants::MOD_JOYSTICK_CONTROL_RELATIVE:
      //controlLoops[1] = joyControlLoop;
      break;
    case NovaConstants::MOD_KEYBOARD_MOUSE_CONTROL:
      //controlLoops[1] = joyControlLoop;
      break;
    case NovaConstants::MOD_DISTANCE_AVOIDANCE:
      controlLoops[1] = distanceAvoidControlLoop;
      break;
    case NovaConstants::MOD_FACE_DETECTION:
      controlLoops[1] = faceDetectionControlLoop;
      break;
    default:
      controlLoops[1] = joyControlLoop; //TODO if something is wrong, default to standard joystick control?
      break;
  }
}

void loop() {
  comm->run();

  NovaCommand *cmd = comm->readCommand(); // now processes one command per loop

  if(cmd != NULL && cmd->modulecode == NovaConstants::MOD_STATUS_NOVA) {
    handleCommands(cmd);
    delete cmd;
  }

  for(AbstractControlLoop* loop : controlLoops) {
    loop->run(cmd);
  }
  //joyControlLoop->run();
  //distanceAvoidControlLoop->run();
  //faceDetectionControlLoop->run(cmd);
  //statusPublishLoop->run(cmd);
}
