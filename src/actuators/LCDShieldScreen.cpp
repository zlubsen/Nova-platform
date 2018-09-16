#include "LCDShieldScreen.h"

LCDShieldScreen::LCDShieldScreen(Adafruit_RGBLCDShield* shield) {
  _lcd = shield;
}

void LCDShieldScreen::setCursor(char column, char line) {
  _lcd->setCursor(static_cast<uint8_t>(column), static_cast<uint8_t>(line));
}

void LCDShieldScreen::print(String text) {
  _lcd->print(text);
}

void LCDShieldScreen::setBacklight(char status) {
  _lcd->setBacklight(static_cast<uint8_t>(status));
}

Adafruit_RGBLCDShield* LCDShieldScreen::getLCD() {
    return _lcd;
}
