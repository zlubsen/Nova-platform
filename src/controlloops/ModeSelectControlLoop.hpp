#ifndef ControlLoop_Mode_Select_hpp
#define ControlLoop_Mode_Select_hpp

#include "config/HardwareConfig.h"
#include "config/NovaConfig.h"
#include <sensors/LCDShieldButtons.h>
#include <actuators/LCDShieldScreen.h>
#include <controlloops/AbstractControlLoop.hpp>
#include <controlloops/StatusPublishLoop.hpp>
#include <controlloops/JoystickAbsoluteControlLoop.hpp>
#include <controlloops/JoystickRelativeControlLoop.hpp>
#include <controlloops/ExternalInputControlLoop.hpp>
#include <controlloops/KeepDistanceControlLoop.hpp>
#include <controlloops/TrackObjectControlLoop.hpp>
#include <FrequencyTimer.h>

#include <vector>

class ModeSelectControlLoop : public AbstractControlLoop {
  public:
    ModeSelectControlLoop(HardwareConfig*  hardwareConfig, NovaConfig* novaConfig);
    void run(NovaProtocolCommand* cmd);
    std::vector<AbstractControlLoop*>* getActiveControlLoops();
    StatusPublishLoop* statusPublishLoop;
    ModeSelectControlLoop* modeSelectControlLoop;

  private:
    void setupControlLoops();
    void setupLCDScreen(NovaConfig* novaConfig);
    void handleCommands(NovaProtocolCommand* cmd);
    void handleButtons();
    void navigateModeSelectMenuUp();
    void navigateModeSelectMenuDown();
    void navigateMessageMenuLeft();
    void navigateMessageMenuRight();
    void setMode(uint8_t mode);
    void switchControlLoop(uint8_t mode);
    void showSelectScreen();
    void showStatusScreen();
    void updateStatusScreen();
    void handleUpdateLCD();
    HardwareConfig* _hardwareConfig;
    NovaConfig* _novaConfig;
    LCDShieldButtons* _buttons;
    LCDShieldScreen* _lcd;
    uint8_t _selectedEntry = 0;
    uint8_t _currentMode = 0;
    bool _lcd_status_mode = true;
    FrequencyTimer* _lcd_menu_timeout_timer;
    FrequencyTimer* _lcd_status_update_timer;

    std::vector<AbstractControlLoop*> _activeControlLoops;

    // TODO make these texts a configuration item - possibly tied to the comm protocol in NovaConstants, or merge into _availableControlLoops
    std::vector<std::string> _controlLoopDescriptions{
      "1 Joystick - abs",
      "2 Joystick - rel",
      "3 External input",
      "4 Keep distance ",
      "5 Track object  ",
    };
};

#endif
