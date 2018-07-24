#include <Arduino.h>
#include "ControlLoop/JoystickControlLoop.h"
#include "ControlLoop/DistanceAvoidControlLoop.h"
#include "ControlLoop/FaceDetectionControlLoop.h"

HardwareConfig* hardwareConfig;
NovaConfig* novaConfig;

JoystickControlLoop* joyControlLoop;
DistanceAvoidControlLoop* distanceAvoidControlLoop;
FaceDetectionControlLoop* faceDetectionControlLoop;

joystick_status left;
joystick_status right;

void setup() {
  Serial.begin(9600);

  hardwareConfig = new HardwareConfig();
  novaConfig = new NovaConfig();

  joyControlLoop = new JoystickControlLoop(hardwareConfig, novaConfig);
  distanceAvoidControlLoop = new DistanceAvoidControlLoop(hardwareConfig, novaConfig);
  faceDetectionControlLoop = new FaceDetectionControlLoop(hardwareConfig, novaConfig);

  hardwareConfig->servo1->goToMiddle();
  hardwareConfig->servo2->goToMiddle();
  hardwareConfig->servo3->goToMiddle();
  hardwareConfig->servo4->goToMiddle();
  hardwareConfig->servo5->goToMiddle();
}

void loop() {
    //joyControlLoop->run();
    distanceAvoidControlLoop->run();
    //faceDetectionControlLoop->run();

    delay(30);
}
