#include "claw.h"
#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include "DFRobot_VL53L0X.h"

#include "m101_hardware_config.h"

Claw::Claw()
{
  Serial.print("Claw Setup...");
  Serial.flush();
  pinMode(PIN_MAGNETIC_SENSOR, INPUT);
  pinMode(PIN_TOUCH_SENSOR, OUTPUT);
  pinMode(PIN_SERVO_CLAW, OUTPUT);
  pinMode(PIN_SERVO_LIFT, OUTPUT);
  Serial.println("Done!");
  Serial.flush();
}
