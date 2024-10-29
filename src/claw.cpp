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
}

Servo Claw::GetServo(Purpose pur)
//function allocates the pins of the arduino to the servos based on their intended purpose
//input: (Lift/Pinch)
//Returns Servo object which can be used in functions such as servolift.write(90)
//output: Servo object - servolift/servopinch
{
  Servo servolift,servopinch;
  switch (pur)
  {
  case Purpose::Lift:
    
    servolift.attach(PIN_SERVO_LIFT);
    return servolift;

  case Purpose::Pinch:

    servopinch.attach(PIN_SERVO_CLAW);
    return servopinch;
  }
}

void Claw::ServoDrop()
//Drops the claw arm into default position
{
  Servo servolift,servopinch;
  servolift = GetServo(Lift);
  servopinch = GetServo(Pinch);

  delay(1000);
  servolift.write(40); 
  delay(2000);
  servopinch.write(180);
  delay(3000);
}

void Claw::ServoPickup() //-----------> CONSIDER writing in ultrasonicboxcheck() functionality, and a pinch-to-unpinch... 
                         //...sequence to move the box into the right position for the ultrasonic
//pinches and lifts claw arm
{
  Servo servolift,servopinch;
  servolift = GetServo(Lift);
  servopinch = GetServo(Pinch);

  delay(1000);
  servopinch.write(140);//clamp ------> if each clamp loosens the servo, we can make it so that every following clamp squeezes more: pos = (150-10*x) or something
  delay(3000);

  //LED sequence for contamination
  TrashDetectionSeq();

  servolift.write(0); //lift box off the ground
  delay(2000);

  //turn off contamination detection LEDs
  mLeds->Set(PIN_CONTAMINATION_LED,false);
  mLeds->Set(PIN_NO_CONTAMINATION_LED,false);
}


bool Claw::ReadMagnetic() //-> used in TrashDetectionSeq command
//detects if magnetic sensor gets activated or not
//returns boolean value (True if magnetic!)
{
  int val = digitalRead(PIN_MAGNETIC_SENSOR);
  if (val == HIGH) {
    return true;
  } else {
    return false;
  }
}

void Claw::TrashDetectionSeq() //-> used during ServoPickup command
//turns on LED if is magnetic 
{
  mLeds->Set(PIN_CONTAMINATION_LED,ReadMagnetic());
  mLeds->Set(PIN_NO_CONTAMINATION_LED,!ReadMagnetic());
}