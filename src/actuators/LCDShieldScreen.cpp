#include "LCDShieldScreen.h"

LCDShieldScreen::LCDShieldScreen(Adafruit_RGBLCDShield* shield) {
  _lcd = shield;
  _lcd->begin(16, 2);
}

void LCDShieldScreen::setCursor(char column, char line) {
  _lcd->setCursor(static_cast<uint8_t>(column), static_cast<uint8_t>(line));
}

void LCDShieldScreen::print(std::string text) {
  _lcd->print(text.c_str());
}

void LCDShieldScreen::setBacklight(char status) {
  _lcd->setBacklight(static_cast<uint8_t>(status));
}

Adafruit_RGBLCDShield* LCDShieldScreen::getLCD() {
    return _lcd;
}
