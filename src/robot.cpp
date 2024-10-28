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
  mMapping = new Mapping;

  while(!mStartButton->Read()){}

  Route = mMapping->FetchRoute(Mapping::Node::Start, Mapping::Node::Factory1);
  SetInitialSpeed();
}

void Robot::Loop()
{
  //Blue LED code
  FollowLine();
  /*
  if (!Junction){
    FollowLine();
    Junction = (mLineSensors->Read(LineSensors::Location::WideLeft) || mLineSensors->Read(LineSensors::Location::WideRight));
  } 
  else{
    if(Route[Count] == Mapping::Direction::Left){
      mMotors->Turn(Motors::Turning::Lefty);
    } 
    else if(Route[Count] == Mapping::Direction::Right){
      mMotors->Turn(Motors::Turning::Righty);
    }
    //To be changed if code for Turn() is made loopable
    Junction = (mLineSensors->Read(LineSensors::Location::WideLeft) || mLineSensors->Read(LineSensors::Location::WideRight));
    if(Junction == false){
      Count += 1;
    }
  }*/
}

void Robot::SetInitialSpeed()
{
  mMotors->SetSpeed(Motors::Location::Left, 200);
  mMotors->SetSpeed(Motors::Location::Right, 200);
  for (int loc = 0; loc < static_cast<int>(Motors::Location::Count); loc++)
  {
    mMotors->Run(static_cast<Motors::Location>(loc), Motors::Direction::Backward); 
  }
}

void Robot::FollowLine()
{
  int Slow = 175, Fast = 200;

  bool LeftLineSensorWhite = (mLineSensors->Read(LineSensors::Location::MidLeft) == LineSensors::Background::White);
  bool RightLineSensorWhite = (mLineSensors->Read(LineSensors::Location::MidRight) == LineSensors::Background::White);
  
  if (!LeftLineSensorWhite && RightLineSensorWhite){
    mMotors->SetSpeed(Motors::Location(0), Fast);
    mMotors->SetSpeed(Motors::Location(1), Slow);
  }
  else if(LeftLineSensorWhite && !RightLineSensorWhite){
    mMotors->SetSpeed(Motors::Location(0), Slow);
    mMotors->SetSpeed(Motors::Location(1), Fast);
  }
  else{
    mMotors->SetSpeed(Motors::Location(0), Fast);
    mMotors->SetSpeed(Motors::Location(1), Fast);
  }
}
