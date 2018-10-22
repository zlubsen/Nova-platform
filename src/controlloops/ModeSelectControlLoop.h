#ifndef ControlLoop_Mode_Select_h
#define ControlLoop_Mode_Select_h

#include "config/HardwareConfig.h"
#include "config/NovaConfig.h"
#include <sensors/LCDShieldButtons.h>
#include <actuators/LCDShieldScreen.h>
#include <controlloops/AbstractControlLoop.h>
#include <controlloops/StatusPublishLoop.h>
#include <controlloops/JoystickAbsoluteControlLoop.h>
#include <controlloops/JoystickRelativeControlLoop.h>
#include <controlloops/ExternalInputControlLoop.h>
#include <controlloops/DistanceAvoidControlLoop.h>
#include <controlloops/FaceDetectionControlLoop.h>
#include <FrequencyTimer.h>

#include <vector>

class ModeSelectControlLoop : public AbstractControlLoop {
  public:
    ModeSelectControlLoop(HardwareConfig*  hardwareConfig, NovaConfig* novaConfig);
    void run(NovaCommand* cmd);
    std::string getLCDStatusString();
    std::vector<AbstractControlLoop*>* getActiveControlLoops();
    StatusPublishLoop* statusPublishLoop;
    ModeSelectControlLoop* modeSelectControlLoop;

  private:
    void setupControlLoops(HardwareConfig* hardwareConfig, NovaConfig* novaConfig);
    void setupLCDScreen(NovaConfig* novaConfig);
    void handleCommands(NovaCommand* cmd);
    void handleButtons();
    void navigateModeSelectMenuUp();
    void navigateModeSelectMenuDown();
    void setMode(int mode);
    void showSelectScreen();
    void showStatusScreen();
    void updateStatusScreen();
    void handleUpdateLCD();
    HardwareConfig* _hardwareConfig;
    LCDShieldButtons* _buttons;
    LCDShieldScreen* _lcd;
    uint8_t _selectedEntry = 0;
    int _currentMode = 0;
    bool _lcd_status_mode = true;
    FrequencyTimer* _lcd_menu_timeout_timer;
    FrequencyTimer* _lcd_status_update_timer;

    JoystickAbsoluteControlLoop* joyAbsoluteControlLoop;
    JoystickRelativeControlLoop* joyRelativeControlLoop;
    ExternalInputControlLoop* externalInputControlLoop;
    DistanceAvoidControlLoop* distanceAvoidControlLoop;
    FaceDetectionControlLoop* faceDetectionControlLoop;

    std::vector<AbstractControlLoop*> _activeControlLoops;
    std::vector<AbstractControlLoop*> _availableControlLoops;

    // TODO make these texts a configuration item - possibly tied to the comm protocol in NovaConstants, or merge into _availableControlLoops
    std::vector<std::string> _controlLoopDescriptions{
      "1 Joystick - abs",
      "2 Joystick - rel",
      "3 External input",
      "4 Distance avoid",
      "5 Face detection",
    };
};

#endif
