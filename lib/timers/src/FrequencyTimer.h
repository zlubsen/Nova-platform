#ifndef Timers_FrequencyTimer_h
#define Timers_FrequencyTimer_h

#include <Arduino.h>

class FrequencyTimer {
  public:
    FrequencyTimer(int frequency_ms);
    bool elapsed();
    void setFrequency(int frequency_ms);
    void resetTimer();

  private:
    unsigned long _frequency_ms;
    unsigned long _last_time_ms;
    void setLastTime();
};

#endif
