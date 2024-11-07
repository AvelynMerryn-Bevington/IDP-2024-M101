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

  //Pickup();
}

void Claw::Drop()
{
  // Lower box to the ground
  mLiftServo->write(LiftDown);
  delay(1000);

  // Release box
  mPinchServo->write(PinchOpen);
  delay(1000);

  mLeds->SetCarrying(false, false);
}

bool Claw::Pickup()
{  
  // Grab box
  mPinchServo->write(PinchClosed);
  delay(1000);

  // Lift box off the ground
  mLiftServo->write(LiftUp);
  for (int pos = LiftDown; pos <= LiftUp; pos += 1) {
    mLiftServo->write(pos);
    delay(15);
  }
  delay(1000);

  bool contaminated = (digitalRead(PIN_MAGNETIC_SENSOR) == HIGH);
  mLeds->SetCarrying(true, contaminated);

  return contaminated;
}

void Claw::PushPosition()
{
  
  for (int pos = LiftUp; pos >= pushpos; pos -= 1) {
    mLiftServo->write(pos);
    delay(15);
  }
}