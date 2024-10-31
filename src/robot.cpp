#include "robot.h"
#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include "DFRobot_VL53L0X.h"
#include "motors.h"
#include "claw.h"

void Robot::Init()
{
  Serial.begin(9600);
  while(!Serial){}

  mLeds = new Leds;
  mMotors = new Motors(mLeds);
  mImu = new Imu;
  mLineSensors = new LineSensors;
  mUltrasonic = new Ultrasonic;
  mTof = new Tof;
  mStartButton = new StartButton;
  mMapping = new Mapping;
  mClaw = new Claw;

  while(!mStartButton->Read()){}

  Route = mMapping->FetchRoute(Mapping::Node::Start, Mapping::Node::Factory1);
  //SetInitialSpeed();
  mMotors->Run(Motors::Location::Left, Motors::Direction::Stopped);
  mMotors->Run(Motors::Location::Right, Motors::Direction::Stopped);
}

void Robot::Loop()
{
  mClaw->Pickup();
  mClaw->Drop();
  /*
  mLeds->Loop();

  FollowLine(150, 200);
  
  if (!Junction){
    Junction = CheckForJunction();
    return;
  }

  if (ReadyForTurn)
  {
    //To be changed if Turn() is made loopable
    ReadyForTurn = false;
    Serial.print(Route[RouteCount]);
  
    mMotors->Run(Motors::Location::Left, Motors::Direction::Stopped);
    mMotors->Run(Motors::Location::Right, Motors::Direction::Stopped);
    delay(250);
    mMotors->Run(Motors::Location::Left, Motors::Direction::Forward);
    mMotors->Run(Motors::Location::Right, Motors::Direction::Forward);

    if(Route[RouteCount] == Mapping::Direction::Left){
      Turn(Turning::Lefty);
    } 
    else if(Route[RouteCount] == Mapping::Direction::Right){
      Turn(Turning::Righty);
    }
  }

  Junction = CheckForJunction();
  if (Junction)
    return;

  RouteCount += 1;
  ReadyForTurn = true;
  if (Route[RouteCount] != Mapping::Direction::End)
    return;

  switch(CurrentPurpose)
  {
    case Purpose::CarryingBox:
      if (ContaminatedBox)
        delay(1000);
      mMotors->SetSpeed(Motors::Location::Left, 0);
      mMotors->SetSpeed(Motors::Location::Right, 0);
      //mClaw->Drop();

    case Purpose::FetchingBox:
      while (true){
        FollowLine(100, 150);
        if (mUltrasonic->BoxCheck())
          break;
    
    case Purpose::ReturningToStart:
      break;
    }
  }
  Serial.println("End");
  CurrentLocation = CurrentDestination;
  Turn(Turning::About);
  ChangingPurpose();
  Route = SelectingDestination(false);
  RouteCount = 0;
  */
}

void Robot::SetInitialSpeed()
{
  for (int loc = 0; loc < static_cast<int>(Motors::Location::Count); loc++)
  {
    mMotors->SetSpeed(static_cast<Motors::Location>(loc), 200);
    mMotors->Run(static_cast<Motors::Location>(loc), Motors::Direction::Forward); 
  }
}

void Robot::FollowLine(const int Slow = 150, const int Fast = 200)
{
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
  return ((mLineSensors->Read(LineSensors::Location::WideLeft) == LineSensors::Background::White) ||
          (mLineSensors->Read(LineSensors::Location::WideRight) == LineSensors::Background::White));
}

void Robot::ChangingPurpose()
{
  if (CurrentLocation == Mapping::Node::Factory1)
    CurrentPurpose = Robot::Purpose::CarryingBox;
  else if (BoxDeliveredCount < Mapping::Node::NumNodes)
    CurrentPurpose = Robot::Purpose::FetchingBox;
  else
    CurrentPurpose = Robot::Purpose::ReturningToStart;
}


std::array<Mapping::Direction, 10> Robot::SelectingDestination(bool Contaminated = false)
{
  switch (CurrentPurpose)
  {
  case Robot::Purpose::ReturningToStart:
    CurrentDestination = Mapping::Node::Start;
    break;

  case Robot::Purpose::FetchingBox:
    CurrentDestination = Mapping::Node::Factory1;
    break;

  case Robot::Purpose::CarryingBox:
    if (Contaminated)
      CurrentDestination = Mapping::Node::ContaminationSite;
    else
    {
      CurrentDestination = Mapping::Node(6+BoxDeliveredCount);
      BoxDeliveredCount += 1;
    }
    break;
  }

  return mMapping->FetchRoute(CurrentLocation, CurrentDestination);
}

void Robot::Turn(Turning direction)
{
  mMotors->Run(Motors::Location::Left, Motors::Direction::Stopped);
  mMotors->Run(Motors::Location::Right, Motors::Direction::Stopped);

  mMotors->SetSpeed(Motors::Location::Left, 200);
  mMotors->SetSpeed(Motors::Location::Right, 200);
  
  //Is this delay necessary?
  delay(500);
  Serial.print("Turning");

  LineSensors::Location readLocation1, readLocation2;
  switch (direction)
  {
  case Turning::Lefty:
    mMotors->Run(Motors::Location::Left, Motors::Direction::Backward);
    mMotors->Run(Motors::Location::Right, Motors::Direction::Forward);
    readLocation1 = LineSensors::Location::MidRight;
    readLocation2 = LineSensors::Location::WideLeft;
    break;
/*
  case Turning::Lefty1:
    mMotors->Run(Motors::Location::Left, Motors::Direction::Backward);
    mMotors->Run(Motors::Location::Right, Motors::Direction::Forward);
    readLocation1 = LineSensors::Location::WideLeft;
    break;
*/
  case Turning::Righty:
    mMotors->Run(Motors::Location::Left, Motors::Direction::Forward);
    mMotors->Run(Motors::Location::Right, Motors::Direction::Backward);
    readLocation1 = LineSensors::Location::MidLeft;
    readLocation2 = LineSensors::Location::WideRight;
    break;
/*
  case Turning::Righty1:
    mMotors->Run(Motors::Location::Left, Motors::Direction::Forward);
    mMotors->Run(Motors::Location::Right, Motors::Direction::Backward);
    readLocation = LineSensors::Location::WideLeft;
    break;
*/
  case Turning::About:
    mMotors->Run(Motors::Location::Left, Motors::Direction::Forward);
    mMotors->Run(Motors::Location::Right, Motors::Direction::Backward);
    readLocation1 = LineSensors::Location::MidRight;

    while (mLineSensors->Read(readLocation1) != LineSensors::Background::White){
      delay(10);
      Serial.println("Stage2");
    }
    break;
  }
  while (mLineSensors->Read(readLocation1) != LineSensors::Background::Black && mLineSensors->Read(readLocation2) != LineSensors::Background::Black) {
    delay(10);
    Serial.println("Stage1");
  }
  while (mLineSensors->Read(readLocation1) != LineSensors::Background::White && mLineSensors->Read(readLocation2) != LineSensors::Background::White) {
    delay(10);
    Serial.println("Stage2");
  }

  mMotors->Run(Motors::Location::Left, Motors::Direction::Forward);
  mMotors->Run(Motors::Location::Right, Motors::Direction::Forward);
}
