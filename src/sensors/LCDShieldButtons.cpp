#include "LCDShieldButtons.h"
#include <Wire.h>

LCDShieldButtons::LCDShieldButtons(Adafruit_RGBLCDShield* shield) {
  _buttons = shield;
}

// Returns an array with the pressed buttons, and the number of buttons pressed as an int.
void LCDShieldButtons::readButtons(uint8_t* num_pressed, Buttons* pressed_buttons) {
  uint8_t buttons = _buttons->readButtons();
  uint8_t pressed_count = 0;

  if (buttons) {
    if (buttons & BUTTON_UP) {
      *pressed_buttons = Buttons::UP;
      pressed_count += 1;
    }
    if (buttons & BUTTON_DOWN) {
      *pressed_buttons = Buttons::DOWN;
      pressed_count += 1;
    }
    if (buttons & BUTTON_LEFT) {
      *pressed_buttons = Buttons::LEFT;
      pressed_count += 1;
    }
    if (buttons & BUTTON_RIGHT) {
      *pressed_buttons = Buttons::RIGHT;
      pressed_count += 1;
    }
    if (buttons & BUTTON_SELECT) {
      *pressed_buttons = Buttons::SELECT;
      pressed_count += 1;
    }
  }
  *num_pressed = pressed_count;
}
