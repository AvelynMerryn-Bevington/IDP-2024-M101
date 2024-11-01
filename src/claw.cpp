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
  mPinchServo->write(PinchOpen);
  mPinchServo->attach(PIN_SERVO_CLAW);
  delay(1000);

  mLiftServo = new Servo();
  mLiftServo->write(LiftDown);
  mLiftServo->attach(PIN_SERVO_LIFT);
  delay(1000);

  mLiftServo->detach();
  mLiftServo->detach();
}

void Claw::Drop()
{
  mLiftServo->attach(PIN_SERVO_LIFT);
  mPinchServo->attach(PIN_SERVO_CLAW);

  // Lower box to the ground
  mLiftServo->write(LiftDown);
  delay(1000);

  // Release box
  mPinchServo->write(PinchOpen);
  delay(1000);

  mLiftServo->detach();
  mPinchServo->detach();

  mLeds->SetCarrying(false, false);
}

bool Claw::Pickup()
{
  mLiftServo->attach(PIN_SERVO_LIFT);
  mPinchServo->attach(PIN_SERVO_CLAW);
  
  // Grab box
  mPinchServo->write(PinchClosed);
  delay(1000);

  // Lift box off the ground
  mLiftServo->write(LiftUp);
  delay(1000);

  bool contaminated = (digitalRead(PIN_MAGNETIC_SENSOR) == HIGH);
  mLeds->SetCarrying(true, contaminated);

  mLiftServo->detach();
  mPinchServo->detach();

  return contaminated;
}
