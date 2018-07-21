#include <Arduino.h>
#include "ControlLoop/JoystickControlLoop.h"

HardwareConfig* hardwareConfig;
NovaConfig* novaConfig;

JoystickControlLoop* joyControlLoop;

joystick_status left;
joystick_status right;

void setup() {
  Serial.begin(9600);

  hardwareConfig = new HardwareConfig();
  novaConfig = new NovaConfig();
  joyControlLoop = new JoystickControlLoop(hardwareConfig, novaConfig);

  Serial.println(joyControlLoop->_filterconst_left.x);
  Serial.println(joyControlLoop->_filterconst_left.y);
  Serial.println(joyControlLoop->_filterconst_right.x);
  Serial.println(joyControlLoop->_filterconst_right.y);

  hardwareConfig->servo1->goToMiddle();
  hardwareConfig->servo2->goToMiddle();
  hardwareConfig->servo3->goToMiddle();
  hardwareConfig->servo4->goToMiddle();
  hardwareConfig->servo5->goToMiddle();
}

void loop() {
    Serial.println("loop!");

    joyControlLoop->run();

    Serial.println(joyControlLoop->_joy_left_output.x, DEC);
    Serial.println(joyControlLoop->_joy_left_output.y, DEC);
    //Serial.println(joyControlLoop->_joy_left_input.sw, DEC);
    Serial.println(joyControlLoop->_joy_right_output.x, DEC);
    Serial.println(joyControlLoop->_joy_right_output.y, DEC);
    //Serial.println(joyControlLoop->_joy_right_output.sw, DEC);

    /*NovaServo* servo = hardwareConfig->servo1;

    for(int i = servo->getDegree(); i > servo->getMinimum(); i--) {
      servo->setDegree(i);
      delay(20);
    }

    for(int i = servo->getMinimum(); i < servo->getMaximum(); i++) {
      servo->setDegree(i);
      delay(20);
    }
    servo->goToMiddle();*/

    delay(30);
}
