#include "robot.h"
#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include "DFRobot_VL53L0X.h"

void Robot::Init()
{
  Serial.begin(9600);
  while(!Serial){}

  mMotors = new Motors;
  mLeds = new Leds;
  mClaw = new Claw;
  mImu = new Imu;
  mLineSensors = new LineSensors;
  mStartButton = new StartButton;

  while(!mStartButton->Read()){

  }
  SetInitialSpeed();
}

void Robot::Loop()
{
  FollowLine();
}

void Robot::SetInitialSpeed()
{
  for (int loc = 0; loc < static_cast<int>(Motors::Location::Count); loc++)
  {
    mMotors->SetSpeed(static_cast<Motors::Location>(loc), 100);
    mMotors->Run(static_cast<Motors::Location>(loc), Motors::Direction::Backward); 
  }
}

void Robot::FollowLine()
{
  mImu->GetYawAngle();
  
  if (mLineSensors->Read(LineSensors::Location::BackLeft) == LineSensors::Background::Black)
    mMotors->AdjustSpeed(Motors::Location::Left, 5);

  if (mLineSensors->Read(LineSensors::Location::BackRight) == LineSensors::Background::Black)
    mMotors->AdjustSpeed(Motors::Location::Right, 5);
}
