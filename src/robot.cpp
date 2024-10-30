#include "robot.h"
#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include "DFRobot_VL53L0X.h"
#include "motors.h"

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
  CheckForJunction();
  
  if (!Junction){
    Junction = (mLineSensors->Read(LineSensors::Location::WideLeft) || mLineSensors->Read(LineSensors::Location::WideRight));
  } 
  else{
    //Turn Left or Right or neither depending on next direction on Route
    if (ReadyForTurn){
      if(Route[RouteCount] == Mapping::Direction::Left){
        mMotors->Turn(Motors::Turning::Lefty);
      } 
      else if(Route[RouteCount] == Mapping::Direction::Right){
        mMotors->Turn(Motors::Turning::Righty);
      }
      ReadyForTurn = false;
    }
    //To be changed if code for Turn() is made loopable
    //Check for junction, if junction is no longer detected we know that we have left it successfully
    Junction = CheckForJunction();
    if(Junction == false){
      RouteCount += 1;
      ReadyForTurn = true;

      if (Route[RouteCount] == Mapping::Direction::End){
        //Code for dealing with destination
        SelectingDestination();
        //Set Destination 
      }
    }
  }
}

void Robot::SetInitialSpeed()
{
  mMotors->SetSpeed(Motors::Location::Left, 200);
  mMotors->SetSpeed(Motors::Location::Right, 200);
  for (int loc = 0; loc < static_cast<int>(Motors::Location::Count); loc++)
  {
    mMotors->Run(static_cast<Motors::Location>(loc), Motors::Direction::Forward); 
  }
}

void Robot::FollowLine()
{
  int Slow = 150, Fast = 200;

  bool LeftLineSensorWhite = (mLineSensors->Read(LineSensors::Location::MidLeft) == LineSensors::Background::White);
  bool RightLineSensorWhite = (mLineSensors->Read(LineSensors::Location::MidRight) == LineSensors::Background::White);
  
  if (!LeftLineSensorWhite && RightLineSensorWhite){
    mMotors->SetSpeed(Motors::Location::Left, Fast);
    mMotors->SetSpeed(Motors::Location::Right, Slow);
  }
  else if(LeftLineSensorWhite && !RightLineSensorWhite){
    mMotors->SetSpeed(Motors::Location::Left, Slow);
    mMotors->SetSpeed(Motors::Location::Right, Fast);
  }
  else{
    mMotors->SetSpeed(Motors::Location::Left, Fast);
    mMotors->SetSpeed(Motors::Location::Right, Fast);
  }
}



bool Robot::CheckForJunction()
{
  bool LeftWideSensorWhite = (mLineSensors->Read(LineSensors::Location::WideLeft) == LineSensors::Background::White);
  bool RightWideSensorWhite = (mLineSensors->Read(LineSensors::Location::WideRight) == LineSensors::Background::White);

  return(LeftWideSensorWhite || RightWideSensorWhite);
}

std::array<Mapping::Direction, 10> Robot::SelectingDestination(bool Contaminated = false)
{
  CurrentLocation = CurrentDestination;

  if (CurrentPurpose == Robot::Purpose::ReturningToStart){
    CurrentDestination = Mapping::Node::Start;
  }
  else if (CurrentPurpose == Robot::Purpose::FetchingBox){
    CurrentDestination = Mapping::Node::Factory1;
  }
  else if (CurrentPurpose == Robot::Purpose::CarryingBox && Contaminated){
    CurrentDestination = Mapping::Node::ContaminationSite;
  }
  else{
    CurrentDestination = Mapping::Node(6+BoxDeliveredCount);
    BoxDeliveredCount += 1;
  }
  return mMapping->FetchRoute(CurrentLocation, CurrentDestination);
}