#include "line_sensors.h"
#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include "DFRobot_VL53L0X.h"

#include "m101_hardware_config.h"

LineSensors::LineSensors()
{
  Serial.print("Line Sensors Setup...");
  Serial.flush();

  for (int loc = 0; loc < (int) Location::Count; loc++)
    pinMode(GetSensor((Location)loc), INPUT);

  Serial.println("Done!");
  Serial.flush();
}

LineSensors::Background LineSensors::Read(Location loc)
{
  return (digitalRead(GetSensor(loc)) == HIGH) ? Background::White : Background::Black; 
}

int LineSensors::GetSensor(Location loc)
{
  switch (loc)
  {
    case Location::BackLeft:
      return PIN_LEFT_BACK_LINE_SENSOR;
    case Location::BackRight:
      return PIN_RIGHT_BACK_LINE_SENSOR;
    case Location::FrontLeft:
      return PIN_LEFT_FRONT_LINE_SENSOR;
    case Location::FrontRight:
      return PIN_RIGHT_FRONT_LINE_SENSOR;
    default:
      return UINT8_MAX; // Error
  }
}
