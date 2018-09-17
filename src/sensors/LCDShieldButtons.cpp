#include "LCDShieldButtons.h"
#include <Wire.h>

LCDShieldButtons::LCDShieldButtons(Adafruit_RGBLCDShield* shield) {
  _buttons = shield;
}

Buttons LCDShieldButtons::readButtons() {
  uint8_t buttons = _buttons->readButtons();
  if (buttons) {
    if (buttons & BUTTON_UP) {
      return Buttons::UP;
    }
    if (buttons & BUTTON_DOWN) {
      return Buttons::DOWN;
    }
    if (buttons & BUTTON_LEFT) {
      return Buttons::LEFT;
    }
    if (buttons & BUTTON_RIGHT) {
      return Buttons::RIGHT;
    }
    if (buttons & BUTTON_SELECT) {
      return Buttons::SELECT;
    }
  }
}
