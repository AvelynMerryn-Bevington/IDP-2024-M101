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

  Servo *mPinchServo = new Servo();
  mPinchServo->attach(PIN_SERVO_CLAW);
  Servo *mLiftServo = new Servo();
  mPinchServo->attach(PIN_SERVO_LIFT);
  ServoDrop();
}

void Claw::ServoDrop()
//Drops the claw arm into default position
{
  delay(1000);
  mLiftServo->write(40); 
  delay(2000);
  mPinchServo->write(180);
  delay(3000);

  //turn off contamination detection LEDs
  mLeds->SetCarrying(false, false);
}

bool Claw::ServoPickup() //-----------> CONSIDER writing in ultrasonicboxcheck() functionality, and a pinch-to-unpinch... 
                         //...sequence to move the box into the right position for the ultrasonic
//pinches and lifts claw arm
//Returns whether box is contaminated
{
  delay(1000);
  mPinchServo->write(140);//clamp ------> if each clamp loosens the servo, we can make it so that every following clamp squeezes more: pos = (150-10*x) or something
  delay(3000);

  mLiftServo->write(0); //lift box off the ground
  delay(2000);

  //turn off contamination detection LEDs
  bool contaminated = (digitalRead(PIN_MAGNETIC_SENSOR) == HIGH);
  mLeds->SetCarrying(true, contaminated);

  return contaminated;
}
