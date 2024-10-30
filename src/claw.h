#ifndef CLAW_H
#define CLAW_H

#include <Servo.h>
#include "leds.h"

class Claw
{
public:
  enum Purpose
  {
    Pinch,
    Lift
  };

  Claw();

  void ServoDrop();
  void ServoPickup();

  int pinchopen = 45,pinchclosed = 8,liftup = 55,liftdown = 37;

private:
  Servo GetServo(Purpose pur);
  void TrashDetectionSeq();
  bool ReadMagnetic();

  Leds *mLeds;
};

#endif CLAW_H