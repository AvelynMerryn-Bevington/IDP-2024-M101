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
  int PinchOpen = 0;
  int PinchClosed = 30;
  int LiftUp = 50;
  int LiftDown = 37;

};

#endif CLAW_H