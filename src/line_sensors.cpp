#include "line_sensors.h"
#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include "DFRobot_VL53L0X.h"

#include "m101_hardware_config.h"

LineSensors::LineSensors()
{
  Serial.print("Line Sensors Setup...");
  Serial.flush();
  pinMode(PIN_LEFT_WIDE_LINE_SENSOR, INPUT);
  pinMode(PIN_RIGHT_WIDE_LINE_SENSOR, INPUT);
  pinMode(PIN_LEFT_MID_LINE_SENSOR, INPUT);
  pinMode(PIN_RIGHT_MID_LINE_SENSOR, INPUT);
  Serial.println("Done!");
  Serial.flush();
}

LineSensors::Background LineSensors::Read(Location loc)
{
  return digitalRead((pin_size_t)loc+2) ? Background::White : Background::Black;
}


