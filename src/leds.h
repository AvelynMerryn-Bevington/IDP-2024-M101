#ifndef LEDS_H
#define LEDS_H

class Leds
{
public:
  Leds();
  void SetCarrying(bool carrying, bool contaminated);
  void BlueLight();
  void TurnOffBlueLight();

private:
  void Set(int Led, bool isOn);

  bool BlueOn;
  bool BlueInit;
  unsigned long time1;
};

#endif // LEDS_H
