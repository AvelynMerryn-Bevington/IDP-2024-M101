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
  mUltrasonic = new Ultrasonic;
  mTof = new Tof;
  mStartButton = new StartButton;

  while(!mStartButton->Read()){}

  SetInitialSpeed();
}

void Robot::Loop()
{
  FollowLine();
}

void Robot::SetInitialSpeed()
{
  mMotors->SetSpeed(Motors::Location::Left, 200);
  mMotors->SetSpeed(Motors::Location::Right, 250);
  for (int loc = 0; loc < static_cast<int>(Motors::Location::Count); loc++)
  {
    mMotors->Run(static_cast<Motors::Location>(loc), Motors::Direction::Backward); 
  }
}

void Robot::FollowLine()
{
  int FastRight = 250, SlowRight = 0;

  bool LeftLineSensorWhite = (mLineSensors->Read(LineSensors::Location::BackLeft) == LineSensors::Background::White);
  bool RightLineSensorWhite = (mLineSensors->Read(LineSensors::Location::BackRight) == LineSensors::Background::White);
  
  if (!LeftLineSensorWhite && RightLineSensorWhite){
    mMotors->SetSpeed(Motors::Location(1), SlowRight);
    Serial.println("Adjusting To The Right");
  }
  else{
    mMotors->SetSpeed(Motors::Location(1), FastRight);
    Serial.println("Adjusting To The Left");
  }
}
