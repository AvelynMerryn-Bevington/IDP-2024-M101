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

  //while(!mStartButton->Read()){}

  //SetInitialSpeed();
}

void Robot::Loop()
{
  FollowLine();
}

void Robot::SetInitialSpeed()
{
  mMotors->SetSpeed(Motors::Location(0), 255);
  mMotors->SetSpeed(Motors::Location(1), 243);
  for (int loc = 0; loc < static_cast<int>(Motors::Location::Count); loc++)
  {
    mMotors->Run(static_cast<Motors::Location>(loc), Motors::Direction::Backward); 
  }
}

void Robot::FollowLine()
{
  int SlowLeft = 100, SlowRight = 100;
  Serial.println(mLineSensors->Read(LineSensors::Location::BackRight));
  /*
  Serial.print(LineSensors::Location::BackLeft);
  Serial.println(mLineSensors->Read(LineSensors::Location::BackLeft));
  Serial.println(LineSensors::Location::BackRight);
  Serial.println(mLineSensors->Read(LineSensors::Location::BackRight));
  
  if (mLineSensors->Read(LineSensors::Location::BackLeft) == LineSensors::Background::Black){
    mMotors->AdjustSpeed(Motors::Location(1), SlowRight);
    Serial.println("Adjusting Right");
  }
  else if (mLineSensors->Read(LineSensors::Location::BackRight) == LineSensors::Background::Black){
    mMotors->AdjustSpeed(Motors::Location(0), SlowLeft);
    Serial.println("Adjusting Left");
  }
  else{
    mMotors->AdjustSpeed(Motors::Location(0), 255);
    mMotors->AdjustSpeed(Motors::Location(1), 243);
    Serial.println("Straight");
  }
  */
}
