#ifndef ControlLoop_StatusPublish_hpp
#define ControlLoop_StatusPublish_hpp

#include <config/HardwareConfig.h>
#include <config/NovaConstants.h>
#include <controlloops/AbstractControlLoop.hpp>
#include <FrequencyTimer.h>
#include <ProtocolLogic.hpp>

class StatusPublishLoop : public AbstractControlLoop {
  public:
    StatusPublishLoop(HardwareConfig* hardwareConfig, int frequency_ms);
    void run(NovaProtocolCommand* cmd);

  private:
    void writeStatusCommand(uint8_t operation, int arg);
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
