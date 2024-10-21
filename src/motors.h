#ifndef MOTORS_H
#define MOTORS_H

#include <Adafruit_MotorShield.h>

class Motors
{
public:
  enum Location
  {
    Left,
    Right,
    Count
  };

  enum Direction
  {
    Forward,
    Backward,
    Stopped
  };

  Motors();

  void Run(const Location loc, const Direction direction);
  void SetSpeed(const Location loc, const uint8_t speed);
  void AdjustSpeed(const Location loc, const int speedChange);

private:
  Adafruit_MotorShield mMotorShield;
  Adafruit_DCMotor* GetMotor(Location loc);
  uint8_t mMotorSpeeds[Location::Count];
};

#endif // MOTORS_H