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

  hardwareConfig->servo1->goToMiddle();
  hardwareConfig->servo2->goToMiddle();
  hardwareConfig->servo3->goToMiddle();
  hardwareConfig->servo4->goToMiddle();
  hardwareConfig->servo5->goToMiddle();
}

void loop() {
    joyControlLoop->run();

    delay(30);
}
