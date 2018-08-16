#ifndef Nova_Sensor_UltraSoundSensor_h
#define Nova_Sensor_UltraSoundSensor_h

#include <Arduino.h>

class UltraSoundSensor {
  public:
    UltraSoundSensor(int trigPin, int echoPin);
    int measureDistance();

  private:
    int _trig_pin;
    int _echo_pin;
    const int _reset_delay = 2; // TODO move these three to NovaConfig ?
    const int _measure_delay = 10;
    const double _dur_dist_conversion = 0.034/2;
};

#endif
