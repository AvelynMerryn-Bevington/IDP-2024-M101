#include "motors.h"
#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include "DFRobot_VL53L0X.h"

#include "m101_hardware_config.h"

Motors::Motors()
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

void Motors::Run(const Location loc, const Direction direction, Leds *leds)
{
  Adafruit_DCMotor *motor = GetMotor(loc);

  switch (direction)
  {
  case Forward:
    motor->run(FORWARD);
    leds->SetMoving(true);
    break;

  case Backward:
    motor->run(BACKWARD);
    leds->SetMoving(true);
    break;

  case Stopped:
    motor->run(BRAKE);
    leds->SetMoving(false);
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
  int newSpeed = mMotorSpeeds[loc] + speedChange;
  if (newSpeed < 0)
  {
    Serial.println("Speed capped at lower-limit of 0.");
    newSpeed = 0;
  }
  if (newSpeed > 255)
  {
    Serial.println("Speed capped at upper-limit max speed.");
    newSpeed = 255;
  }
  SetSpeed(loc, (uint8_t)newSpeed);
}
