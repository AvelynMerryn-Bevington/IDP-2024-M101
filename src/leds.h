#ifndef LEDS_H
#define LEDS_H

class Leds
{
public:
  Leds();
  void SetCarrying(bool carrying, bool contaminated);
  void SetMoving(bool moving);
  void Set(int Led, bool isOn);
  void Leds::BlueFlash();
  

private:
  
};

#endif // LEDS_H
