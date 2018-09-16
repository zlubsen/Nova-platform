#ifndef Nova_Sensor_LCDShieldButtons_h
#define Nova_Sensor_LCDShieldButtons_h

#include <Adafruit_RGBLCDShield.h>

enum class Buttons {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  SELECT
};

class LCDShieldButtons {
  public:
    LCDShieldButtons(Adafruit_RGBLCDShield* shield);
    void readButtons(uint8_t* num_pressed, Buttons* pressed_buttons);

  private:
    Adafruit_RGBLCDShield* _buttons;
};

#endif
