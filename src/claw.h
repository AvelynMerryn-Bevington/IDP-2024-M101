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
  bool ServoPickup();

private:
  bool TrashDetectionSeq();
  bool ReadMagnetic();

  Servo *mPinchServo;
  Servo *mLiftServo;
  Leds *mLeds;
};

#endif CLAW_H