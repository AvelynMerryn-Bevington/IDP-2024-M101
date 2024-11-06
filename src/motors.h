#ifndef MOTORS_H
#define MOTORS_H

#include <Adafruit_MotorShield.h>
#include "line_sensors.h"
#include "leds.h"

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

  Motors(Leds *leds);

  void Run(const Location loc, const Direction direction);
  void SetSpeed(const Location loc, const uint8_t speed);
  void AdjustSpeed(const Location loc, const int speedChange);

private:
  Adafruit_MotorShield mMotorShield;
  Adafruit_DCMotor* GetMotor(Location loc);
  uint8_t mMotorSpeeds[Location::Count];

  const double LeftWheelCalibrationFactor = 0.98;

  LineSensors *mLineSensors;
  Leds *mLeds;
};

#endif // MOTORS_H