#include <Arduino.h>
#include "ModeSelectControlLoop.h"

ModeSelectControlLoop::ModeSelectControlLoop(HardwareConfig* hardwareConfig, NovaConfig* novaConfig) {
  _hardwareConfig = hardwareConfig; // TODO would rather not have a local pointer to the whole hardwareconfig thing
  _lcd = hardwareConfig->lcdScreen;
  _buttons = hardwareConfig->selectButtons;
  setupControlLoops(hardwareConfig, novaConfig);

  _selectedEntry = 0;
}

void ModeSelectControlLoop::setupControlLoops(HardwareConfig* hardwareConfig, NovaConfig* novaConfig) {
  statusPublishLoop = new StatusPublishLoop(hardwareConfig, novaConfig->_status_publish_frequency_ms);
  modeSelectControlLoop = this;
  joyAbsoluteControlLoop = new JoystickAbsoluteControlLoop(hardwareConfig, novaConfig);
  joyRelativeControlLoop = new JoystickRelativeControlLoop(hardwareConfig, novaConfig);
  externalInputControlLoop = new ExternalInputControlLoop(hardwareConfig, novaConfig);
  distanceAvoidControlLoop = new DistanceAvoidControlLoop(hardwareConfig, novaConfig);
  faceDetectionControlLoop = new FaceDetectionControlLoop(hardwareConfig, novaConfig);

  _availableControlLoops[0] = joyAbsoluteControlLoop;
  _availableControlLoops[1] = joyRelativeControlLoop;
  _availableControlLoops[2] = externalInputControlLoop;
  _availableControlLoops[3] = distanceAvoidControlLoop;
  _availableControlLoops[4] = faceDetectionControlLoop;

  _activeControlLoops[0] = statusPublishLoop;
  _activeControlLoops[1] = modeSelectControlLoop;
  _activeControlLoops[2] = joyAbsoluteControlLoop;
}

void ModeSelectControlLoop::handleCommands(NovaCommand* cmd) {
  if(cmd->operandcode == NovaConstants::OP_STATUS_SEND_SET_MODE) {
    // disable/reenable input to servos to minimize servo jitter
    _hardwareConfig->suspendServos();
    setMode(cmd->arg1 - 1); // TODO this is a nasty way to align the novaconstants with the array index used in this class
    _hardwareConfig->activateServos();
  }
}

void ModeSelectControlLoop::handleButtons() {
  Buttons pressed = _buttons->readButtons();
  switch (pressed) {
    case Buttons::UP:
      navigateModeSelectMenuUp();
      break;
    case Buttons::DOWN:
      navigateModeSelectMenuDown();
      break;
    case Buttons::LEFT:
      break;
    case Buttons::RIGHT:
      break;
    case Buttons::SELECT:
      setMode(_selectedEntry);
      break;
    default:
      break;
  }
}

void ModeSelectControlLoop::navigateModeSelectMenuUp() {
  _selectedEntry--;
  if(_selectedEntry < 0)
    _selectedEntry = 4;

  updateScreen();
}

void ModeSelectControlLoop::navigateModeSelectMenuDown() {
  _selectedEntry++;
  if(_selectedEntry > 4)
    _selectedEntry = 0;

  updateScreen();
}

void ModeSelectControlLoop::updateScreen() {
  _lcd->setCursor(0,0);
  _lcd->print(_controlLoopDescriptions[_selectedEntry]);
}

void ModeSelectControlLoop::setMode(int mode) {
  _activeControlLoops[2] = _availableControlLoops[mode];
}

void ModeSelectControlLoop::run(NovaCommand* cmd) {
  if(cmd != NULL && cmd->modulecode == NovaConstants::MOD_STATUS_NOVA) {
    handleCommands(cmd);
  }

  handleButtons();
}
