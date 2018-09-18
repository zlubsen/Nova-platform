#include "FrequencyTimer.h"

FrequencyTimer::FrequencyTimer(int frequency_ms) {
  setFrequency(frequency_ms);
  setLastTime();
}

void FrequencyTimer::setLastTime() {
  _last_time_ms = millis();
}

bool FrequencyTimer::elapsed() {
  unsigned long current_time_ms = millis();

  if(current_time_ms - _last_time_ms > _frequency_ms) {
    setLastTime();
    return true;
  }

  return false;
}

void FrequencyTimer::resetTimer() {
  setLastTime();
}

void FrequencyTimer::setFrequency(int frequency_ms) {
  _frequency_ms = frequency_ms;
}
