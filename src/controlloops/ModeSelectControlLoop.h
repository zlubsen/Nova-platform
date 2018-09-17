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

class ModeSelectControlLoop : public AbstractControlLoop {
  public:
    ModeSelectControlLoop(HardwareConfig*  hardwareConfig, NovaConfig* novaConfig);
    void run(NovaCommand* cmd);
  private:
    void setupControlLoops(HardwareConfig* hardwareConfig, NovaConfig* novaConfig);
    void handleCommands(NovaCommand* cmd);
    void setMode(int mode);
    void handleButtons();
    void navigateModeSelectMenuUp();
    void navigateModeSelectMenuDown();
    void updateScreen();
    HardwareConfig* _hardwareConfig;
    LCDShieldButtons* _buttons;
    LCDShieldScreen* _lcd;
    uint8_t _selectedEntry;

    StatusPublishLoop* statusPublishLoop;
    ModeSelectControlLoop* modeSelectControlLoop;
    JoystickAbsoluteControlLoop* joyAbsoluteControlLoop;
    JoystickRelativeControlLoop* joyRelativeControlLoop;
    ExternalInputControlLoop* externalInputControlLoop;
    DistanceAvoidControlLoop* distanceAvoidControlLoop;
    FaceDetectionControlLoop* faceDetectionControlLoop;

    AbstractControlLoop* _activeControlLoops[3];
    AbstractControlLoop* _availableControlLoops[5];
    
    String _controlLoopDescriptions[5] = {
      "1 Joystick - abs",
      "2 Joystick - rel",
      "3 External input",
      "4 Distance avoid",
      "5 Face detection",
    };
};

#endif
