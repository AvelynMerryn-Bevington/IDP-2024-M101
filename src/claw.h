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

private:
  Servo *mPinchServo;
  Servo *mLiftServo;
  Leds *mLeds;
  int PinchOpen = 10;
  int PinchClosed = 50;
  int LiftUp = 125;
  int LiftDown = 100;
};

#endif CLAW_H