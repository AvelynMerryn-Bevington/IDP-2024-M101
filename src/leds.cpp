#include "leds.h"
#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include "DFRobot_VL53L0X.h"

#include "m101_hardware_config.h"

Leds::Leds()
{
  Serial.print("LED Setup...");
  Serial.flush();

  pinMode(PIN_CONTAMINATION_LED, OUTPUT);
  pinMode(PIN_NO_CONTAMINATION_LED, OUTPUT);
  pinMode(PIN_DRIVING_STATUS_LED, OUTPUT);
  
  Serial.println("Done!");
  Serial.flush();
}

void Leds::Set(int Led, bool isOn)
{
  digitalWrite(Led, isOn ? HIGH : LOW);
}

void Leds::SetCarrying(bool carrying, bool contaminated)
{
  Set(PIN_CONTAMINATION_LED, carrying && contaminated);
  Set(PIN_NO_CONTAMINATION_LED, carrying && !contaminated);
}

void Leds::SetMoving(bool moving)
{
  Set(PIN_DRIVING_STATUS_LED, moving);
}
