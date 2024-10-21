#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include "DFRobot_VL53L0X.h"

#include "robot.h"

Robot robot;

void setup()
{
  robot.Init();
}

void loop()
{
  robot.Loop();
}
