#include "start_button.h"
#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include "DFRobot_VL53L0X.h"

#include "m101_hardware_config.h"

StartButton::StartButton()
{
  Serial.print("Start Button Setup...");
  Serial.flush();
  pinMode(PIN_START_BUTTON, INPUT);
  Serial.println("Done!");
  Serial.flush();
}

bool StartButton::Read()
{
  return digitalRead(pin_size_t(PIN_START_BUTTON));
}
