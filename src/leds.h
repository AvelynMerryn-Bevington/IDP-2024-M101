#ifndef LEDS_H
#define LEDS_H

class Leds
{
public:
  Leds();
  void Loop();
  void SetCarrying(bool carrying, bool contaminated);
  void SetMoving(bool moving);
  void BlueLight();


private:
  void Set(int Led, bool isOn);

  bool mMoving;
  bool mMovingOn;
  unsigned long mMovingFlashMillis;
  
  bool BlueOn;
  bool BlueInit;
  unsigned long time1;
};

#endif // LEDS_H
