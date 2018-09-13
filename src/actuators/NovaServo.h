#ifndef Nova_Actuator_Servo_h
#define Nova_Actuator_Servo_h

#include <Servo.h>

class NovaServo {
  private:
    int _pin;
    Servo _servo;
    int _min;
    int _max;

  public:
    NovaServo(int pin);
    NovaServo(int pin, int min, int max);
    void setAllowedRange(int min, int max);
    int getMinimum();
    int getMaximum();
    void goToMinimum();
    void goToMaximum();
    void goToMiddle();
    void setDegree(int degree);
    int getDegree();
    void attach();
    void detach();
};

#endif
