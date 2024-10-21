#ifndef LEDS_H
#define LEDS_H

class Leds
{
public:
  Leds();
  void SetCarrying(bool carrying, bool contaminated);
  void SetMoving(bool moving);

private:
  void Set(int Led, bool isOn);
};

#endif // LEDS_H
