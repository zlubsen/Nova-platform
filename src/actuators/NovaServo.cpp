#include <Arduino.h>
#include "NovaServo.h"

NovaServo::NovaServo(int pin) : NovaServo(pin, 0, 180) {
}

NovaServo::NovaServo(int pin, int min, int max) {
  _pin = pin;
  setAllowedRange(min, max);
  goToMiddle();
  //_servo.attach(_pin);
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

int NovaServo::getMiddle() {
  return ( ( _max - _min ) / 2 ) + _min;
}

void NovaServo::goToMinimum() {
  setDegree(_min);
}

void NovaServo::goToMaximum() {
  setDegree(_max);
}

void NovaServo::goToMiddle() {
  setDegree(getMiddle());
}

void NovaServo::setDegree(int degree) {
  if(degree < _min)
    _servo.write(_min);
  else if(degree > _max)
    _servo.write(_max);
  else
    _servo.write(degree);
}

void NovaServo::setDegreeSmooth(int degree) {
  const int step_size = 10; // TODO make configuration item
  int current_degree = getDegree();
  int diff = current_degree - degree;
  diff = abs(diff);

  int no_of_steps = diff / step_size;

  for(int steps = no_of_steps; steps > 0; steps--) {
    if(current_degree < degree)
      current_degree += step_size;
    else
      current_degree -= step_size;

    setDegree(current_degree);
    delay(50); // TODO make configuration item
  }

  goToMiddle();
}

int NovaServo::getDegree() {
  return _servo.read();
}

void NovaServo::attach() {
  _servo.attach(_pin);
}

void NovaServo::detach() {
  setDegreeSmooth(getMiddle());
  _servo.detach();
}
