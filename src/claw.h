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
  
  
  
private:
  Servo GetServo(Purpose pur);
  void TrashDetectionSeq();
  bool ReadMagnetic();

  Leds *mLeds;

};

#endif CLAW_H
