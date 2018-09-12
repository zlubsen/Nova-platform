#include <Arduino.h>
#include <communication/SerialCommunication.h>
#include <config/HardwareConfig.h>
#include <config/NovaConfig.h>
#include <config/NovaConstants.h>
#include <controlloops/AbstractControlLoop.h>
#include <controlloops/StatusPublishLoop.h>
#include <controlloops/JoystickAbsoluteControlLoop.h>
#include <controlloops/JoystickRelativeControlLoop.h>
#include <controlloops/ExternalInputControlLoop.h>
#include <controlloops/DistanceAvoidControlLoop.h>
#include <controlloops/FaceDetectionControlLoop.h>

void handleCommands(NovaCommand*);
void setMode(int);

HardwareConfig* hardwareConfig;
NovaConfig* novaConfig;

SerialCommunication* comm;

AbstractControlLoop* controlLoops[2];

StatusPublishLoop* statusPublishLoop;

JoystickAbsoluteControlLoop* joyAbsoluteControlLoop;
JoystickRelativeControlLoop* joyRelativeControlLoop;
ExternalInputControlLoop* externalInputControlLoop;
DistanceAvoidControlLoop* distanceAvoidControlLoop;
FaceDetectionControlLoop* faceDetectionControlLoop;

joystick_status left;
joystick_status right;

void setup() {
  hardwareConfig = new HardwareConfig();
  novaConfig = new NovaConfig();

  comm = hardwareConfig->comm;

  statusPublishLoop = new StatusPublishLoop(hardwareConfig, novaConfig->_status_publish_frequency_ms);
  joyAbsoluteControlLoop = new JoystickAbsoluteControlLoop(hardwareConfig, novaConfig);
  joyRelativeControlLoop = new JoystickRelativeControlLoop(hardwareConfig, novaConfig);
  externalInputControlLoop = new ExternalInputControlLoop(hardwareConfig, novaConfig);
  distanceAvoidControlLoop = new DistanceAvoidControlLoop(hardwareConfig, novaConfig);
  faceDetectionControlLoop = new FaceDetectionControlLoop(hardwareConfig, novaConfig);

  controlLoops[0] = statusPublishLoop;
  controlLoops[1] = externalInputControlLoop; //TODO default control loop is now face detection?
}

void handleCommands(NovaCommand* cmd) {
  if(cmd->operandcode == NovaConstants::OP_STATUS_SEND_SET_MODE) {
    setMode(cmd->arg1);
  }
}

void setMode(int mode) {
  // pause input to servos
  switch (mode) {
    case NovaConstants::MOD_JOYSTICK_CONTROL_ABOLUTE:
      controlLoops[1] = joyAbsoluteControlLoop;
      break;
    case NovaConstants::MOD_JOYSTICK_CONTROL_RELATIVE:
      controlLoops[1] = joyRelativeControlLoop;
      break;
    case NovaConstants::MOD_EXTERNAL_INPUT_CONTROL:
      controlLoops[1] = externalInputControlLoop;
      break;
    case NovaConstants::MOD_DISTANCE_AVOIDANCE:
      controlLoops[1] = distanceAvoidControlLoop;
      break;
    case NovaConstants::MOD_FACE_DETECTION:
      controlLoops[1] = faceDetectionControlLoop;
      break;
    default:
      controlLoops[1] = joyAbsoluteControlLoop; //TODO if something is wrong, default to standard joystick control?
      break;
  }
  // resume input to servos
}

void loop() {
  comm->run();

  NovaCommand *cmd = comm->readCommand(); // TODO now processes one command per loop

  if(cmd != NULL && cmd->modulecode == NovaConstants::MOD_STATUS_NOVA) {
    handleCommands(cmd);
  }

  for(AbstractControlLoop* loop : controlLoops) {
    loop->run(cmd);
  }

  delete cmd;
}
