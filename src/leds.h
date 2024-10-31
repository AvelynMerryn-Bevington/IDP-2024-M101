#ifndef LEDS_H
#define LEDS_H

class Leds
{
public:
  Leds();
  void Loop();
  void SetCarrying(bool carrying, bool contaminated);
  void SetMoving(bool moving);

private:
  void Set(int Led, bool isOn);

  bool mMoving;
  bool mMovingOn;
  unsigned long mMovingFlashMillis;
};

#endif // LEDS_H
