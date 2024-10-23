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

};

#endif CLAW_H
