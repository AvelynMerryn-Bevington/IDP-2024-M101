#include "claw.h"
#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include "DFRobot_VL53L0X.h"

#include "m101_hardware_config.h"

#include <Servo.h> //for servos

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



Servo Claw::GetServo(Purpose pur)
//function allocates the pins of the arduino to the servos based on their intended purpose
//Returns Servo object which can be used in functions such as servolift.write(90)
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
//returns nothing atm
{
  Servo servolift,servopinch;
  servolift = GetServo(Lift);
  servopinch = GetServo(Pinch);

  delay(1000);
  servolift.write(40); 
  delay(2000);
  servopinch.write(90);
  delay(3000);
}

void Claw::ServoPickup()
//pinches and lifts claw arm
//returns nothing atm
{
  Servo servolift,servopinch;
  servolift = GetServo(Lift);
  servopinch = GetServo(Pinch);

  delay(1000);
  servopinch.write(125);//clamp ------> if each clamp loosens the servo, we can make it so that every following clamp squeezes more: pos = (150-10*x) or something
  delay(3000);//time for clamp to close
  servolift.write(0); //lift box off the ground
  delay(2000); //wait for lift
}
