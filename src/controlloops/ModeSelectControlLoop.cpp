#include <Arduino.h>
#include "ModeSelectControlLoop.h"

ModeSelectControlLoop::ModeSelectControlLoop(HardwareConfig* hardwareConfig, NovaConfig* novaConfig) {
  _hardwareConfig = hardwareConfig; // TODO would rather not have a local pointer to the whole hardwareconfig thing
  _lcd = hardwareConfig->lcdScreen;
  _buttons = hardwareConfig->selectButtons;
  setupControlLoops(hardwareConfig, novaConfig);
  setupLCDScreen(novaConfig);
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

  // TODO use of this array would be the preffered way of maintaining the active controlloops (instead of in main.cpp)
  /*_activeControlLoops[0] = statusPublishLoop;
  _activeControlLoops[1] = modeSelectControlLoop;
  _activeControlLoops[2] = joyAbsoluteControlLoop;*/
  activeControlLoop = _availableControlLoops[_currentMode];
}

void ModeSelectControlLoop::setupLCDScreen(NovaConfig* novaConfig) {
  _lcd_menu_timeout_timer = new FrequencyTimer(novaConfig->_lcd_menu_timeout_ms);
  _lcd->setBacklight(novaConfig->_lcd_menu_background_color);
  showStatusScreen();
}

void ModeSelectControlLoop::handleCommands(NovaCommand* cmd) {
  if(cmd->operandcode == NovaConstants::OP_STATUS_SEND_SET_MODE) {
    int mode = cmd->arg1 - 1; // TODO this is a nasty way to align the novaconstants with the array index used in this class
    setMode(mode);
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
    case Buttons::NONE_PRESSED:
    default:
      // nothing pressed, do nothing
      break;
  }
}

//TODO make min/max index numbers constants/defines
void ModeSelectControlLoop::navigateModeSelectMenuUp() {
  if(_selectedEntry == 0)
    _selectedEntry = 4;
  else
    _selectedEntry--;

  showSelectScreen();
}

//TODO make min/max index numbers constants/defines
void ModeSelectControlLoop::navigateModeSelectMenuDown() {
  if(_selectedEntry == 4)
    _selectedEntry = 0;
  else
    _selectedEntry++;

  showSelectScreen();
}

void ModeSelectControlLoop::showSelectScreen() {
  _lcd->setCursor(0,0);
  _lcd->print("Select mode.....");
  _lcd->setCursor(0,1);
  _lcd->print(_controlLoopDescriptions[_selectedEntry]);

  _lcd_menu_timeout_timer->resetTimer();
}

void ModeSelectControlLoop::showStatusScreen() {
  _lcd->setCursor(0,0);
  _lcd->print("NOVA is in mode:");
  _lcd->setCursor(0,1);
  _lcd->print(_controlLoopDescriptions[_currentMode]);
}

void ModeSelectControlLoop::setMode(int mode) {
  // disable/reenable input to servos to minimize servo jitter
  _hardwareConfig->suspendServos();

  activeControlLoop = _availableControlLoops[mode];
  _currentMode = mode;

  showStatusScreen();

  _hardwareConfig->activateServos();
}

void ModeSelectControlLoop::run(NovaCommand* cmd) {
  if(cmd != NULL && cmd->modulecode == NovaConstants::MOD_STATUS_NOVA) {
    handleCommands(cmd);
  }

  handleButtons();

  if(_lcd_menu_timeout_timer->elapsed()) {
    showStatusScreen();
    _selectedEntry = _currentMode;
  }
}
