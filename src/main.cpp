#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Arduino_LSM6DS3.h>
#include <MadgwickAHRS.h>

#include "m101_hardware_config.h"
#include "DFRobot_VL53L0X.h"


// ------------------------------- IMU ------------------------------- //

void Imu_Setup()
{
  Madgwick madgwickFilter;

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

  filter.updateIMU(gx, gy, gz, ax, ay, az);
  Serial.print("Yaw = ");  Serial.print(dtostrf(filter.getYaw(), 4, 0, buffer)); Serial.println(" °");
}

// ------------------------------- LEDs ------------------------------- //

void Led_Setup()
{
  pinMode(PIN_CONTAMINATION_LED, OUTPUT);
  pinMode(PIN_NO_CONTAMINATION_LED, OUTPUT);
  pinMode(PIN_DRIVING_STATUS_LED, OUTPUT);
}

void Led_Set(int LedPin, bool TurnedOn)
{
  digitalWrite(LedPin, TurnedOn);
}

void Led_UpdateStatus()
{
  bool carryingBox = ???;
  bool contaminated = ???;

  Led_Set(PIN_CONTAMINATION_LED, carryingBox && contaminated);
  Led_Set(PIN_NO_CONTAMINATION_LED, carryingBox && !contaminated);
}

// ------------------------------- MOTORS ------------------------------- //

// NEEDS TO BE MOVED TO A CLASS AND STORED NON-GLOBALLY
Adafruit_MotorShield AdafruitMotorShield = Adafruit_MotorShield();

Adafruit_DCMotor* Motors_GetMotor(int motor)
{
  return AdafruitMotorShield.getMotor(motor);
}

void Motors_Run(int motor, bool forward)
{
  Motors_GetMotor(motor)->run(forward);
}

void Motors_SetSpeed(int motor, int speed)
{
  Motors_GetMotor(motor)->setSpeed(speed);
}

// ------------------------------- LINE FOLLOWING ------------------------------- //

void LineSensors_Setup()
{
  pinMode(PIN_LEFT_FRONT_LINE_SENSOR, INPUT);
  pinMode(PIN_RIGHT_FRONT_LINE_SENSOR, INPUT);
  pinMode(PIN_LEFT_BACK_LINE_SENSOR, INPUT);
  pinMode(PIN_RIGHT_BACK_LINE_SENSOR, INPUT);
}

bool LineSensors_Read(int sensor)
{
  return digitalRead(sensor) == HIGH;
}

void FollowLine()
{
  const int fastSpeed = 150;
  const int slowSpeed = 100;

  int rightMotorSpeed = LineSensors_Read(PIN_LEFT_BACK_LINE_SENSOR) ? slowSpeed : fastSpeed;
  Motors_SetSpeed(MOTOR_RIGHT, rightMotorSpeed);

  int leftMotorSpeed = LineSensors_Read(PIN_RIGHT_BACK_LINE_SENSOR) ? slowSpeed : fastSpeed;
  Motors_SetSpeed(MOTOR_LEFT, leftMotorSpeed);

  Motors_Run(MOTOR_LEFT, forward=true);
  Motors_Run(MOTOR_RIGHT, forward=true);
}

// ------------------------------- BOX LOADING ------------------------------- //

void Claw_Setup()
{
  pinMode(PIN_MAGNETIC_SENSOR, INPUT);
  pinMode(PIN_TOUCH_SENSOR, OUTPUT);
  pinMode(PIN_SERVO_CLAW, OUTPUT);
  pinMode(PIN_SERVO_LIFT, OUTPUT);
}

// ------------------------------- MAIN ------------------------------- //

void setup()
{
  Serial.begin(9600);
  while(!Serial){} // Wait for the serial to set up

  Led_Setup();
  LineSensors_Setup();
  Claw_Setup();
  Imu_Setup();
}

void loop()
{
  Led_UpdateStatus();
}
