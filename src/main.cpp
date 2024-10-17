// Imported Libraries

#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Arduino_LSM6DS3.h>
#include <MadgwickAHRS.h>

// Pin Assignment

#define L_FrontLine 2
#define R_FrontLine 3
#define L_BackLine 4
#define R_BackLine 5
#define magSensor 6
#define R_LED 7
#define G_LED 8
#define B_LED 9

#define touchSensor 11
#define servoClaw 12
#define servoLift 13

// Global Variable Assignment

bool Contaminated = false;
bool CarryingBox = false;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
//Motor at M1
Adafruit_DCMotor *MotorRight = AFMS.getMotor(1);
// Motor at M2
Adafruit_DCMotor *MotorLeft= AFMS.getMotor(2);

// Function Assignment
void Task_LedStatusUpdate();


void setup() {
  // Initialisation
  Serial.begin(9600);
  pinMode(L_FrontLine, INPUT);
  pinMode(R_FrontLine, INPUT);
  pinMode(L_BackLine, INPUT);
  pinMode(R_BackLine, INPUT);
  pinMode(R_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(B_LED, OUTPUT);
}

void loop(){
  Task_LedStatusUpdate();
}


void followLine(){ 
  int valLeft = digitalRead(L_BackLine);  // read left input value 

  int valRight = digitalRead(R_BackLine);  // read right input value 
  //accelerate and deccelerate motors

  MotorRight->run(FORWARD);
  MotorLeft->run(FORWARD);
  if (valLeft == LOW) {
    MotorRight->setSpeed(100);
  }
  else if (valLeft == HIGH){
    MotorRight->setSpeed(150);
  }
  if(valRight == LOW){
    MotorLeft->setSpeed(100);
  }
  else if(valRight == HIGH){
    MotorLeft->setSpeed(150);
 
}
}

void Task_LedStatusUpdate(){
  if (CarryingBox){
    if (Contaminated){
      SetLed(R_LED, 1);       // sets the Red LED on
      SetLed(G_LED, 0);       // sets the Green LED on
    } else {
      SetLed(R_LED, 0);         // sets Red LED off
      SetLed(G_LED, 1);        // sets Green LED on
    }
  } else {
    SetLed(R_LED, 0);           // sets Red LED off
    SetLed(G_LED, 0);           // sets Green LED off
  }
}

void SetLed(int LedPin, bool TurnedOn){
  digitalWrite(LedPin, TurnedOn);
}