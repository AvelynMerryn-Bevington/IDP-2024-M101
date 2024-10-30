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
  Servo GetServo(Purpose pur);

  int pinchopen = 45,pinchclosed = 12,liftup = 55,liftdown = 37;

private:
  
  void TrashDetectionSeq();
  bool ReadMagnetic();

  Leds *mLeds;
};

#endif CLAW_H