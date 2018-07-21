#include <Arduino.h>
#include "NovaServo.h"

NovaServo::NovaServo(int pin) : NovaServo(pin, 0, 180) {
}

NovaServo::NovaServo(int pin, int min, int max) {
  _servo.attach(pin);
  _min = min;
  _max = max;
}

void NovaServo::setAllowedRange(int min, int max) {
  _min = min;
  _max = max;
}

int NovaServo::getMinimum() {
  return _min;
}

int NovaServo::getMaximum() {
  return _max;
}

void NovaServo::goToMinimum() {
setDegree(_min);
}

void NovaServo::goToMaximum() {
  setDegree(_max);
}

void NovaServo::goToMiddle() {
  setDegree( ( ( _max - _min ) / 2 ) + _min);
}

void NovaServo::setDegree(int degree) {
  Serial.print(degree);
  Serial.println();

  if(degree < _min)
    _servo.write(_min);
  else if(degree > _max)
    _servo.write(_max);
  else
    _servo.write(degree);
}

int NovaServo::getDegree() {
  return _servo.read();
}
