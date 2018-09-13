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

void setup() {
  hardwareConfig = new HardwareConfig();
  novaConfig = new NovaConfig();

  comm = hardwareConfig->comm;

  statusPublishLoop = new StatusPublishLoop(hardwareConfig, novaConfig->_status_publish_frequency_ms);

  /*
  joyRelativeControlLoop = new JoystickRelativeControlLoop(hardwareConfig, novaConfig);
  externalInputControlLoop = new ExternalInputControlLoop(hardwareConfig, novaConfig);
  distanceAvoidControlLoop = new DistanceAvoidControlLoop(hardwareConfig, novaConfig);
  faceDetectionControlLoop = new FaceDetectionControlLoop(hardwareConfig, novaConfig);
  */

  controlLoops[0] = statusPublishLoop;
  //TODO default control loop is now joystick absolute?
  controlLoops[1] = new JoystickAbsoluteControlLoop(hardwareConfig, novaConfig);
}

void handleCommands(NovaCommand* cmd) {
  if(cmd->operandcode == NovaConstants::OP_STATUS_SEND_SET_MODE) {
    // reset input to servos
    hardwareConfig->resetServosToMiddle();
    setMode(cmd->arg1);
  }
}

void setMode(int mode) {
  AbstractControlLoop* current_mode = controlLoops[1];
  delete current_mode;

  switch (mode) {
    case NovaConstants::MOD_JOYSTICK_CONTROL_ABSOLUTE:
      controlLoops[1] = new JoystickAbsoluteControlLoop(hardwareConfig, novaConfig);//joyAbsoluteControlLoop;
      break;
    case NovaConstants::MOD_JOYSTICK_CONTROL_RELATIVE:
      controlLoops[1] = new JoystickRelativeControlLoop(hardwareConfig, novaConfig);//joyRelativeControlLoop;
      break;
    case NovaConstants::MOD_EXTERNAL_INPUT_CONTROL:
      controlLoops[1] = new ExternalInputControlLoop(hardwareConfig, novaConfig);//externalInputControlLoop;
      break;
    case NovaConstants::MOD_DISTANCE_AVOIDANCE:
      controlLoops[1] = new DistanceAvoidControlLoop(hardwareConfig, novaConfig);//distanceAvoidControlLoop;
      break;
    case NovaConstants::MOD_FACE_DETECTION:
      controlLoops[1] = new FaceDetectionControlLoop(hardwareConfig, novaConfig);//faceDetectionControlLoop;
      break;
    default:
      // TODO if something is wrong, default to standard joystick control?
      controlLoops[1] = new JoystickAbsoluteControlLoop(hardwareConfig, novaConfig);//joyAbsoluteControlLoop;
      break;
  }
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
