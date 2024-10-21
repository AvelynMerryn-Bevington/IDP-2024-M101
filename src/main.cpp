#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Arduino_LSM6DS3.h>
#include <MadgwickAHRS.h>

#include "m101_hardware_config.h"
#include "DFRobot_VL53L0X.h"


// ------------------------------- IMU ------------------------------- //

Madgwick madgwickFilter;

void Imu_Setup()
{
  return;
  Serial.print("LSM6DS3 IMU initialization ");

  if (!IMU.begin())
  {
    Serial.println("FAILED.");
    IMU.end();

    while(true){} // Kill the whole program
  }

  Serial.println("completed successfully.");
  Serial.println();

  madgwickFilter.begin(SAMPLE_RATE_IMU_HZ);
}

void Imu_GetYawAngle()
{
  return;
  char buffer[5];
  float ax, ay, az;
  float gx, gy, gz;
  if (!IMU.accelerationAvailable() ||
      !IMU.gyroscopeAvailable() ||
      !IMU.readAcceleration(ax, ay, az) ||
      !IMU.readGyroscope(gx, gy, gz))
  {
    return;
  }

  madgwickFilter.updateIMU(gx, gy, gz, ax, ay, az);
  Serial.print("Yaw = ");
  Serial.print(dtostrf(madgwickFilter.getYaw(), 4, 0, buffer));
  Serial.println(" °");
}

// ------------------------------- LEDs ------------------------------- //

void Led_Setup()
{
  Serial.print("LED Setup...");
  pinMode(PIN_CONTAMINATION_LED, OUTPUT);
  pinMode(PIN_NO_CONTAMINATION_LED, OUTPUT);
  pinMode(PIN_DRIVING_STATUS_LED, OUTPUT);
  Serial.println("Done!");
}

void Led_Set(int LedPin, bool TurnedOn)
{
  digitalWrite(LedPin, TurnedOn ? HIGH : LOW);
}

void Led_UpdateStatus()
{
  bool carryingBox = false;
  bool contaminated = false;

  Led_Set(PIN_CONTAMINATION_LED, carryingBox && contaminated);
  Led_Set(PIN_NO_CONTAMINATION_LED, carryingBox && !contaminated);
}

// ------------------------------- MOTORS ------------------------------- //

// NEEDS TO BE MOVED TO A CLASS AND STORED NON-GLOBALLY
Adafruit_MotorShield AdafruitMotorShield = Adafruit_MotorShield();
Adafruit_DCMotor* AdafruitMotorRight = AdafruitMotorShield.getMotor(MOTOR_RIGHT);
Adafruit_DCMotor* AdafruitMotorLeft = AdafruitMotorShield.getMotor(MOTOR_LEFT);

uint8_t rightMotorSpeed = 0;
uint8_t leftMotorSpeed = 0;

void Motors_Run(int motor, bool forward)
{
  Serial.print("Motors run...");
  delay(100);

  if (motor == MOTOR_LEFT)
    AdafruitMotorLeft->run(forward ? FORWARD : BACKWARD);
  if (motor == MOTOR_RIGHT)
    AdafruitMotorRight->run(forward ? FORWARD : BACKWARD);

  Serial.println("Done!");
}

void Motors_SetSpeed(int motor, int speed)
{
  Serial.print("Motors set speed...");
  delay(100);

  if (motor == MOTOR_LEFT)
  {
    leftMotorSpeed = speed;
    AdafruitMotorLeft->setSpeed(speed);
  }
  else if (motor == MOTOR_RIGHT)
  {
    rightMotorSpeed = speed;
    AdafruitMotorRight->setSpeed(speed);
  }

  Serial.println("Done!");
}

void Motors_Setup()
{
  Serial.print("Motors setup...");
  if (!AdafruitMotorShield.begin())
  {
    Serial.println("Could not find Motor Shield. Check wiring.");
    while(true){} // Kill the whole entire program
  }
  Serial.println("Done!");

  Serial.print("Motors start...");
  Motors_SetSpeed(MOTOR_RIGHT, 100);
  Motors_SetSpeed(MOTOR_LEFT, 100);
  Motors_Run(MOTOR_LEFT, true);
  Motors_Run(MOTOR_RIGHT, true);
  Serial.println("Done!");
}

// ------------------------------- LINE FOLLOWING ------------------------------- //

void LineSensors_Setup()
{
  Serial.print("Line Sensors Setup...");
  pinMode(PIN_LEFT_FRONT_LINE_SENSOR, INPUT);
  pinMode(PIN_RIGHT_FRONT_LINE_SENSOR, INPUT);
  pinMode(PIN_LEFT_BACK_LINE_SENSOR, INPUT);
  pinMode(PIN_RIGHT_BACK_LINE_SENSOR, INPUT);
  Serial.println("Done!");
}

bool LineSensors_Read(int sensor)
{
  return digitalRead(sensor) == HIGH;
}

void FollowLine()
{
  if (!LineSensors_Read(PIN_LEFT_BACK_LINE_SENSOR))
  {
    rightMotorSpeed += 5;
    Motors_SetSpeed(MOTOR_RIGHT, rightMotorSpeed);
  }

  if (!LineSensors_Read(PIN_RIGHT_BACK_LINE_SENSOR))
  {
    leftMotorSpeed += 5;
    Motors_SetSpeed(MOTOR_LEFT, leftMotorSpeed);
  }

  Motors_Run(MOTOR_RIGHT, true);
  Motors_Run(MOTOR_LEFT, true);
}

// ------------------------------- BOX LOADING ------------------------------- //

void Claw_Setup()
{
  Serial.print("Claw Setup...");
  pinMode(PIN_MAGNETIC_SENSOR, INPUT);
  pinMode(PIN_TOUCH_SENSOR, OUTPUT);
  pinMode(PIN_SERVO_CLAW, OUTPUT);
  pinMode(PIN_SERVO_LIFT, OUTPUT);
  Serial.println("Done!");
}

// ------------------------------- MAIN ------------------------------- //

void setup()
{
  Serial.begin(9600);
  while(!Serial){}
  Serial.println("Initialising...");

  Led_Setup();
  LineSensors_Setup();
  Claw_Setup();
  Imu_Setup();
  Motors_Setup();
  
  Serial.println("Initialised");
}

bool firstRun = true;
void loop()
{
  if (firstRun)
  {
    Serial.println("First loop...");
  }

  FollowLine();
  Led_UpdateStatus();

  if (firstRun)
  {
    Serial.println("First loop done!");
    firstRun = false;
  }
}
