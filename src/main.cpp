#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Arduino_LSM6DS3.h>
#include <MadgwickAHRS.h>

// Imported Libraries

// Global Variable Assignment

bool Contaminated = false;
bool CarryingBox = false;


// Pin Assignment

int L_FrontLine = 2;
int R_FrontLine = 3;
int L_BackLine = 4;
int R_BackLine = 5;
int magSensor = 6;
int R_LED = 7;
int G_LED = 8;
int B_LED = 9;
// int TOF_Sensor = 10;
int touchSensor = 10;
int servoClaw = 11;
int servoLift = 12;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
//Motor at M1
Adafruit_DCMotor *MotorRight = AFMS.getMotor(1);
// Motor at M2
Adafruit_DCMotor *MotorLeft= AFMS.getMotor(2);

// Defines
#define SAMPLE_RATE 10  // in Hz, for IMU
// Constructors
Madgwick filter;  // Madgwick algorithm for roll, pitch, and yaw calculations

// Function Assignment
void HandlingLED();
void PickUpBox();
void followLine();
void getYawAngle();


void setup() {
  // Initialisation
  Serial.begin(9600);
  pinMode(L_FrontLine, INPUT);
  pinMode(R_FrontLine, INPUT);
  pinMode(L_BackLine, INPUT);
  pinMode(R_BackLine, INPUT);
  pinMode(magSensor, INPUT);
  pinMode(R_LED, OUTPUT);          // sets the digital pin 7 as output (Red LED)
  pinMode(G_LED, OUTPUT);          // sets the digital pin 8 as output (Green LED)
  pinMode(B_LED, OUTPUT);          // sets the digital pin 9 as output (Blue LED)
  pinMode(touchSensor, INPUT);
  pinMode(servoClaw, OUTPUT);
  pinMode(servoLift, OUTPUT);


  // IMU stuff
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
    filter.begin(SAMPLE_RATE);  // initialize Madgwick filter

}

void loop(){
  // This is the main part of the program, and this is looped continuously until the reset button is pressed.
  HandlingLED();
}

//Any custom functions go here
void PickUpBox() {

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

void HandlingLED(){
  if (CarryingBox){
    if (Contaminated){
      digitalWrite(7, HIGH);       // sets the Red LED on
      digitalWrite(8, LOW);       // sets the Green LED on
    } else {
      digitalWrite(7, LOW);         // sets Red LED off
      digitalWrite(8, HIGH);        // sets Green LED on
    }
  } else {
    digitalWrite(7, LOW);           // sets Red LED off
    digitalWrite(8, LOW);           // sets Green LED off
  }
}