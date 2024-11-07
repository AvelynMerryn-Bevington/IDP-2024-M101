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
  void Drop();
  bool Pickup();
  void PushPosition();

private:
  Servo *mPinchServo;
  Servo *mLiftServo;
  Leds *mLeds;
  int PinchOpen = 20;
  int PinchClosed = 65;
  int LiftUp = 140;
  int LiftDown = 120;
  int pushpos = LiftDown + 5;
};

#endif CLAW_H
