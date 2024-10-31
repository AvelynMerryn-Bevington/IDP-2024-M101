#include "claw.h"
#include "leds.h"
#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include "DFRobot_VL53L0X.h"
#include <Servo.h>

#include "m101_hardware_config.h"

Claw::Claw()
{
  Serial.print("Claw Setup...");
  Serial.flush();

  pinMode(PIN_MAGNETIC_SENSOR, INPUT);
  pinMode(PIN_SERVO_CLAW, OUTPUT);
  pinMode(PIN_SERVO_LIFT, OUTPUT);

  Serial.println("Done!");
  Serial.flush();

  mPinchServo = new Servo();
  mPinchServo->attach(PIN_SERVO_CLAW);
  mLiftServo = new Servo();
  mPinchServo->attach(PIN_SERVO_LIFT);
  Drop();
}

void Claw::Drop()
{
  delay(1000);

  // Lower box to the ground
  mLiftServo->write(40); 
  delay(2000);

  // Release box
  mPinchServo->write(180);
  delay(3000);

  mLeds->SetCarrying(false, false);
}

bool Claw::Pickup()
{
  delay(1000);
  
  // Grab box
  mPinchServo->write(140);
  delay(3000);

  // Lift box off the ground
  mLiftServo->write(0);
  delay(2000);

  bool contaminated = (digitalRead(PIN_MAGNETIC_SENSOR) == HIGH);
  mLeds->SetCarrying(true, contaminated);

  return contaminated;
}
