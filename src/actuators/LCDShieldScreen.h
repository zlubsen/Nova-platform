#ifndef Nova_Actuator_LCDScreen_h
#define Nova_Actuator_LCDScreen_h

#include <Arduino.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <string>

class LCDShieldScreen {
  public:
    LCDShieldScreen(Adafruit_RGBLCDShield* shield);
    void setCursor(char, char);
    void print(std::string text);
    void setBacklight(char status);
    Adafruit_RGBLCDShield* getLCD();

  private:
    Adafruit_RGBLCDShield* _lcd;
};

#endif
