#include "motors.h"
#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include "DFRobot_VL53L0X.h"
#include "line_sensors.h"

#include "m101_hardware_config.h"

Motors::Motors(Leds *leds)
{
  Serial.print("Motors setup...");
  Serial.flush();
  mMotorShield = Adafruit_MotorShield();

  if (!mMotorShield.begin())
  {
    Serial.println("Could not find Motor Shield. Check wiring.");
    Serial.flush();
    while(true){} // Kill the whole entire program
  }

  Serial.println("Done!");
  Serial.flush();
}

Adafruit_DCMotor* Motors::GetMotor(Location loc)
{
  switch (loc)
  {
  case Location::Left:
    return mMotorShield.getMotor(MOTOR_LEFT);
  
  case Location::Right:
    return mMotorShield.getMotor(MOTOR_RIGHT);

  default:
    return NULL;
  }
}

void Motors::Run(const Location loc, const Direction direction)
{
  Adafruit_DCMotor *motor = GetMotor(loc);

  switch (direction)
  {
  case Forward:
    motor->run(FORWARD);
    mLeds->SetMoving(true);
    break;

  case Backward:
    motor->run(BACKWARD);
    mLeds->SetMoving(true);
    break;

  case Stopped:
    motor->run(RELEASE);
    mLeds->SetMoving(false);
    break;
  }
}

void Motors::SetSpeed(const Location loc, const uint8_t speed)
{
  Adafruit_DCMotor *motor = GetMotor(loc);

  double adjustedSpeedDbl = (double)speed;
  if (loc == Left)
    adjustedSpeedDbl *= LeftWheelCalibrationFactor;

  uint8_t adjustedSpeed = (uint8_t)adjustedSpeedDbl;
  if (adjustedSpeedDbl > 255.0)
    adjustedSpeed = 255;

  mMotorSpeeds[loc] = adjustedSpeed;
  motor->setSpeed(adjustedSpeed);
}
