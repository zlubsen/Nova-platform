#ifndef ControlLoop_StatusPublish_h
#define ControlLoop_StatusPublish_h

#include <config/HardwareConfig.h>
#include <config/NovaConstants.h>
#include <controlloops/AbstractControlLoop.h>
#include <FrequencyTimer.h>

class StatusPublishLoop : public AbstractControlLoop {
  public:
    StatusPublishLoop(HardwareConfig* hardwareConfig, int frequency_ms);
    void run(NovaCommand* cmd);
    std::string getLCDStatusString();

  private:
    SerialCommunication* _comm;
    NovaServo* servo1;
    NovaServo* servo2;
    NovaServo* servo3;
    NovaServo* servo4;
    NovaServo* servo5;
    UltraSoundSensor* ultraSoundSensor;
    FrequencyTimer* _timer;
};

#endif
