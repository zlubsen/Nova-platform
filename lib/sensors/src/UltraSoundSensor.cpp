#include <Arduino.h>
#include <UltraSoundSensor.h>

UltraSoundSensor::UltraSoundSensor(int trigPin, int echoPin) {
  _trig_pin = trigPin;
  _echo_pin = echoPin;

  pinMode(_trig_pin, OUTPUT);
  pinMode(_echo_pin, INPUT);
}

int UltraSoundSensor::measureDistance() {
  long duration;
  int distance;

  digitalWrite(_trig_pin, LOW);
  delayMicroseconds(_reset_delay);
  digitalWrite(_trig_pin, HIGH);
  delayMicroseconds(_measure_delay);
  digitalWrite(_trig_pin, LOW);

  duration = pulseIn(_echo_pin, HIGH);
  distance = duration * _dur_dist_conversion;
  
  return distance;
}
