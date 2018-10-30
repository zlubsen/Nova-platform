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

  _availableControlLoops.push_back(joyAbsoluteControlLoop);
  _availableControlLoops.push_back(joyRelativeControlLoop);
  _availableControlLoops.push_back(externalInputControlLoop);
  _availableControlLoops.push_back(distanceAvoidControlLoop);
  _availableControlLoops.push_back(faceDetectionControlLoop);

  _activeControlLoops.push_back(statusPublishLoop);
  _activeControlLoops.push_back(modeSelectControlLoop);
  _activeControlLoops.push_back(joyAbsoluteControlLoop);
}

void ModeSelectControlLoop::setupLCDScreen(NovaConfig* novaConfig) {
  _lcd_menu_timeout_timer = new FrequencyTimer(novaConfig->_lcd_menu_timeout_ms);
  _lcd_status_update_timer = new FrequencyTimer(novaConfig->_lcd_status_update_ms);
  _lcd->setBacklight(novaConfig->_lcd_menu_background_color);
  showStatusScreen();
}

std::vector<AbstractControlLoop*>* ModeSelectControlLoop::getActiveControlLoops() {
  return &_activeControlLoops;
}

void ModeSelectControlLoop::handleCommands(NovaProtocolCommand* cmd) {
  if(cmd->asset == cmd_module &&
      cmd->operation == cmd_set_mode &&
        cmd->args.size() == 1) {
    int mode = cmd->args.at(0) - 1; // TODO this is a nasty way to align the novaconstants with the array index used in this class
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

void ModeSelectControlLoop::navigateModeSelectMenuUp() {
  if(_selectedEntry == 0)
    _selectedEntry = _controlLoopDescriptions.size()-1;
  else
    _selectedEntry--;

  showSelectScreen();
}

void ModeSelectControlLoop::navigateModeSelectMenuDown() {
  if(_selectedEntry == _controlLoopDescriptions.size()-1)
    _selectedEntry = 0;
  else
    _selectedEntry++;

  showSelectScreen();
}

void ModeSelectControlLoop::showSelectScreen() {
  _lcd_status_mode = false;

  _lcd->setCursor(0,0);
  _lcd->print("Select mode.....");
  _lcd->setCursor(0,1);
  _lcd->print(_controlLoopDescriptions.at(_selectedEntry));

  _lcd_menu_timeout_timer->resetTimer();
}

void ModeSelectControlLoop::showStatusScreen() {
  _lcd_status_mode = true;

  _lcd->setCursor(0,0);
  _lcd->print(_controlLoopDescriptions.at(_selectedEntry));
  updateStatusScreen();
}

void ModeSelectControlLoop::updateStatusScreen() {
  _lcd->setCursor(0,1);
  _lcd->print(_activeControlLoops.at(2)->getLCDStatusString());
}

void ModeSelectControlLoop::setMode(int mode) {
  // disable/reenable input to servos to minimize servo jitter
  _hardwareConfig->suspendServos();

  _activeControlLoops.at(2) = _availableControlLoops[mode];
  _currentMode = mode;

  showStatusScreen();

  _hardwareConfig->activateServos();
}

void ModeSelectControlLoop::run(NovaProtocolCommand* cmd) {
  if(cmd != nullptr && cmd->module == cmd_nova) {
    handleCommands(cmd);
  }

  handleButtons();

  handleUpdateLCD();
}

void ModeSelectControlLoop::handleUpdateLCD() {
  if(_lcd_status_mode && _lcd_status_update_timer->elapsed())
    updateStatusScreen();

  if(!_lcd_status_mode && _lcd_menu_timeout_timer->elapsed()) {
    showStatusScreen();
    _selectedEntry = _currentMode;
  }
}

std::string ModeSelectControlLoop::getLCDStatusString() {
  std::string status(16, ' ');
  return status;
}
