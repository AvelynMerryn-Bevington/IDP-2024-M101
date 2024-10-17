// Imported Libraries

#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Arduino_LSM6DS3.h> // Accesses IMU
#include <MadgwickAHRS.h>    // Calculates yaw angle from IMU
#include "DFRobot_VL53L0X.h" // TOF distance sensor

// Pin Assignment

#define L_Back_LineSensor 2
#define R_Back_LineSensor 3
#define L_Front_LineSensor 4
#define R_Front_LineSensor 5
#define magSensor 6
#define R_LED 7
#define G_LED 8
#define B_LED 9
#define touchSensor 10
#define servoClaw 11
#define servoLift 12

#define SAMPLE_RATE_IMU 10  // in Hz, sample rate of IMU


// Global Variable Assignment

bool Contaminated = false;
bool CarryingBox = false;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield adaFruitMotorShield = Adafruit_MotorShield();
//Motor at M1, right motor
Adafruit_DCMotor *MotorRight = adaFruitMotorShield.getMotor(1);
// Motor at M2, left motor
Adafruit_DCMotor *MotorLeft= adaFruitMotorShield.getMotor(2);

// Function Assignment
void Task_LedStatusUpdate();


void setup() {
  // Initialisation
  Serial.begin(9600);
  pinMode(L_Front_LineSensor, INPUT);
  pinMode(R_Front_LineSensor, INPUT);
  pinMode(L_Back_LineSensor, INPUT);
  pinMode(R_Back_LineSensor, INPUT);
  pinMode(magSensor, INPUT);
  pinMode(R_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(B_LED, OUTPUT);
  pinMode(touchSensor, OUTPUT);
  pinMode(servoClaw, OUTPUT);
  pinMode(servoLift, OUTPUT);

// IMU initialisation
  // Constructors
  Madgwick filter;  // Madgwick algorithm for roll, pitch, and yaw calculations
  while (!Serial);  // wait for serial initialization
      Serial.print("LSM6DS3 IMU initialization ");
      if (IMU.begin()) {  // initialize IMU
          Serial.println("completed successfully.");
      } else {
          Serial.println("FAILED.");
          IMU.end();
          while (1);
      }
      Serial.println();
      filter.begin(SAMPLE_RATE_IMU);  // initialize Madgwick filter

}

void loop(){
  Task_LedStatusUpdate();
}


void followLine(){ 
  int valLeft = digitalRead(L_Back_LineSensor);  // read left input value 

  int valRight = digitalRead(R_Back_LineSensor);  // read right input value 
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

void getYawAngle() {
  char buffer[5];    // string buffer for use with dtostrf() function
   float ax, ay, az;  // accelerometer values
   float gx, gy, gz;  // gyroscope values
   if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()
      && IMU.readAcceleration(ax, ay, az) && IMU.readGyroscope(gx, gy, gz)) {
      filter.updateIMU(gx, gy, gz, ax, ay, az);  // update roll, pitch, and yaw values
      // Print rotation angles
      Serial.print("Yaw = ");  Serial.print(dtostrf(filter.getYaw(), 4, 0, buffer)); Serial.println(" °");
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