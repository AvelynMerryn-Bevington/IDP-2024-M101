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
  
  BlueInit = true;
  BlueOn = false;

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

void Leds::BlueLight()
{
  unsigned long time2;

  if (BlueInit == true) {
    time1 = millis();
  }

  time2 = millis();
  Serial.println(time2);
  Serial.println(time1);

  if (time2 >= time1 + 450) {
    if (BlueOn == true) {
      Set(PIN_DRIVING_STATUS_LED, false);
      BlueOn = false; 
    } else {
      Set(PIN_DRIVING_STATUS_LED, true);
      BlueOn = true;
    }
    time1 = time2; 
  }
  BlueInit = false;
}

void Leds::TurnOffBlueLight()
{
  BlueOn = false;
  Set(PIN_DRIVING_STATUS_LED, false);
}