#include <Arduino.h>
#include "Joystick.h"

#define ACTIVATED LOW

Joystick::Joystick(int pin_x, int pin_y, int pin_switch) {
  _x_pin = pin_x;
  _y_pin = pin_y;
  _switch_pin = pin_switch;

  pinMode(_switch_pin, INPUT_PULLUP);
}

int Joystick::getXPosition() {
  return analogRead(_x_pin);
}

int Joystick::getYPosition() {
  return analogRead(_y_pin);
}

bool Joystick::isSwitchPressed() {
  return (digitalRead(_switch_pin) == ACTIVATED);
}

void Joystick::getStatus(joystick_status* status) {
  status->x = getXPosition();
  status->y = getYPosition();
  status->sw = isSwitchPressed();
}
