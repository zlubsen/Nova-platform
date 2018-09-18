#ifndef Nova_Sensor_LCDShieldButtons_h
#define Nova_Sensor_LCDShieldButtons_h

#include <Adafruit_RGBLCDShield.h>

enum class Buttons {
  UP = 0,
  DOWN,
  LEFT,
  RIGHT,
  SELECT,
  NONE_PRESSED
};

class LCDShieldButtons {
  public:
    LCDShieldButtons(Adafruit_RGBLCDShield* shield);
    Buttons readButtons();

  private:
    Adafruit_RGBLCDShield* _buttons;
};

#endif
