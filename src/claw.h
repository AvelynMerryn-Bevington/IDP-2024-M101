#ifndef CLAW_H
#define CLAW_H

class Claw
{
public:
  enum Purpose
  {
    Pinch,
    Lift
  };

  Claw();

  void Claw::ServoDrop();
  void Claw::ServoPickup();
  
  
  
private:
  Servo GetServo(Purpose pur);
  void Claw::TrashDetectionSeq();
  bool Claw::ReadMagnetic();

  Leds *mLeds;

};

#endif CLAW_H
