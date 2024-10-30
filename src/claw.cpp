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

  Servo servolift,servopinch;
  servolift = GetServo(Lift);
  servopinch = GetServo(Pinch);

  
  servolift.write(liftdown); 
  servopinch.write(pinchopen);
  delay(3000);
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
  int pos;
  servolift = GetServo(Lift);
  servopinch = GetServo(Pinch);

  delay(1000);
  for (pos = liftup; pos >= liftdown; pos -= 1) {
    servolift.write(pos); // tell servo to go to position in variable 'pos'
    delay(15); // waits 15 ms for the servo to reach the position
  }
  delay(2000);
  for (pos = pinchclosed; pos <= pinchopen; pos += 1) {
    servopinch.write(pos); // tell servo to go to position in variable 'pos'
    delay(15); // waits 15 ms for the servo to reach the position
  }
  delay(1000);
}

void Claw::ServoPickup() //-----------> CONSIDER writing in ultrasonicboxcheck() functionality, and a pinch-to-unpinch... 
                         //...sequence to move the box into the right position for the ultrasonic
//pinches and lifts claw arm
{
  Servo servolift,servopinch;
  int pos;
  servolift = GetServo(Lift);
  servopinch = GetServo(Pinch);


  delay(4000);

  //pinch closed
  for (pos = pinchopen; pos >= pinchclosed; pos -= 1) {
    servopinch.write(pos); // tell servo to go to position in variable 'pos'
    delay(15); // waits 15 ms for the servo to reach the position
    Serial.println("pinching!");
  }
  delay(1000);

  //LED sequence for contamination
  TrashDetectionSeq();

  //lift box off the ground
  for (pos = liftdown; pos <= liftup; pos += 1) {
    servolift.write(pos); // tell servo to go to position in variable 'pos'
    delay(15); // waits 15 ms for the servo to reach the position
  }
  delay(1000);

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