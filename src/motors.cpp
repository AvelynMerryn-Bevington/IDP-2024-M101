#include "motors.h"
#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include "DFRobot_VL53L0X.h"

#include "m101_hardware_config.h"

Motors::Motors()
{
  Serial.print("Motors setup...");
  mMotorShield = Adafruit_MotorShield();

  if (!mMotorShield.begin())
  {
    Serial.println("Could not find Motor Shield. Check wiring.");
    while(true){} // Kill the whole entire program
  }
  Serial.println("Done!");
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
    break;

  case Backward:
    motor->run(BACKWARD);
    break;

  case Stopped:
    motor->run(BRAKE);
    break;
  }
}

void Motors::SetSpeed(const Location loc, const uint8_t speed)
{
  Adafruit_DCMotor *motor = GetMotor(loc);

  mMotorSpeeds[loc] = speed;
  motor->setSpeed(speed);
}

void Motors::AdjustSpeed(const Location loc, const int speedChange)
{
  uint8_t newSpeed = static_cast<uint8_t>(static_cast<int>(mMotorSpeeds[loc]) + speedChange);
  SetSpeed(loc, newSpeed);
}
