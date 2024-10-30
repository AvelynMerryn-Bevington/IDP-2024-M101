#ifndef MOTORS_H
#define MOTORS_H

#include <Adafruit_MotorShield.h>
#include "line_sensors.h"

class Motors
{
public:
  enum Location
  {
    Right,
    Left,
    Count
  };

  enum Direction
  {
    Forward,
    Backward,
    Stopped
  };

  enum Turning //Must be different string to enum Location
  {
    Righty,
    Lefty,
    About,
    Lefty1,
    Righty1
  };

  Motors();

  void Run(const Location loc, const Direction direction);
  void SetSpeed(const Location loc, const uint8_t speed);
  void AdjustSpeed(const Location loc, const int speedChange);
  void Motors::Turn(Turning direction); //Does the "Motors::" text need to be in front of the "Turn" here?
  void Motors::Shuffle();

private:
  Adafruit_MotorShield mMotorShield;
  Adafruit_DCMotor* GetMotor(Location loc);
  uint8_t mMotorSpeeds[Location::Count];

  LineSensors *mLineSensors;
};

#endif // MOTORS_H