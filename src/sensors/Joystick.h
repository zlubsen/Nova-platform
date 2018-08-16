#ifndef Nova_Sensor_Joystick_h
#define Nova_Sensor_Joystick_h

typedef struct {
    int x;
    int y;
    bool sw;
  } joystick_status;

class Joystick {
  private:
    int _x_pin;
    int _y_pin;
    int _switch_pin;

  public:
    Joystick(int pin_x, int pin_y, int pin_switch);
    int getXPosition();
    int getYPosition();
    bool isSwitchPressed();
    void getStatus(joystick_status* status);
};

#endif
