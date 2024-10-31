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

  mRoute = mMapping->FetchRoute(Mapping::Node::Start, Mapping::Node::Factory1);
  SetInitialSpeed();
}

void Robot::Loop()
{
  mLeds->Loop();

  FollowLine(150, 200);
  
  if (!mJunction)
  {
    mJunction = CheckForJunction();
    return;
  }

  if (mReadyForTurn)
  {
    mReadyForTurn = false;
  
    mMotors->Run(Motors::Location::Left, Motors::Direction::Stopped);
    mMotors->Run(Motors::Location::Right, Motors::Direction::Stopped);
    delay(250);
    mMotors->Run(Motors::Location::Left, Motors::Direction::Forward);
    mMotors->Run(Motors::Location::Right, Motors::Direction::Forward);

    if(mRoute[0] == Mapping::Direction::Left){
      Turn(Turning::Lefty);
    } 
    else if(mRoute[0] == Mapping::Direction::Right){
      Turn(Turning::Righty);
    }
  }

  mJunction = CheckForJunction();
  if (mJunction)
    return;

  mRoute.erase(mRoute.begin());
  mReadyForTurn = true;
  if (!mRoute.empty())
    return;

  Serial.println("End Of Route");
  delay(1000);

  Turn(Turning::About);
  ChangingPurpose();
  mRoute = SelectingDestination(false);
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
  
  if (!LeftLineSensorWhite && RightLineSensorWhite)
  {
    mMotors->SetSpeed(Motors::Location::Left, Fast);
    mMotors->SetSpeed(Motors::Location::Right, Slow);
  }
  else if(LeftLineSensorWhite && !RightLineSensorWhite)
  {
    mMotors->SetSpeed(Motors::Location::Left, Slow);
    mMotors->SetSpeed(Motors::Location::Right, Fast);
  }
  else
  {
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
  if (mCurrentDestination == Mapping::Node::Factory1)
    mCurrentPurpose = Robot::Purpose::CarryingBox;
  else if (mDeliveredBoxes.size() < 7)
    mCurrentPurpose = Robot::Purpose::FetchingBox;
  else
    mCurrentPurpose = Robot::Purpose::ReturningToStart;
}

Mapping::Node Robot::GetNextDeliveryNode()
{
  for (auto deliveryBay : DeliveryBays)
  {
    bool found = false;
    for (auto bay : mDeliveredBoxes)
    {
      if (bay != deliveryBay)
        continue;
      found = true;
      break;
    }

    if (!found)
      return deliveryBay;
  }
}

::std::vector<Mapping::Direction> Robot::SelectingDestination(bool Contaminated = false)
{
  mCurrentLocation = mCurrentDestination;

  switch (mCurrentPurpose)
  {
  case Robot::Purpose::ReturningToStart:
    mCurrentDestination = Mapping::Node::Start;
    break;

  case Robot::Purpose::FetchingBox:
    mCurrentDestination = Mapping::Node::Factory1;
    break;

  case Robot::Purpose::CarryingBox:
    if (Contaminated)
      mCurrentDestination = Mapping::Node::ContaminationSite;
    else
      mCurrentDestination = GetNextDeliveryNode();
      mDeliveredBoxes.push_back(mCurrentDestination);
    break;
  }

  return mMapping->FetchRoute(mCurrentLocation, mCurrentDestination);
}

void Robot::Turn(Turning direction)
{
  mMotors->Run(Motors::Location::Left, Motors::Direction::Stopped);
  mMotors->Run(Motors::Location::Right, Motors::Direction::Stopped);

  mMotors->SetSpeed(Motors::Location::Left, 200);
  mMotors->SetSpeed(Motors::Location::Right, 200);
  delay(500);

  LineSensors::Location readLocation;
  switch (direction)
  {
  case Turning::Lefty:
    mMotors->Run(Motors::Location::Left, Motors::Direction::Backward);
    mMotors->Run(Motors::Location::Right, Motors::Direction::Forward);
    readLocation = LineSensors::Location::MidRight;
    break;

  case Turning::Lefty1:
    mMotors->Run(Motors::Location::Left, Motors::Direction::Backward);
    mMotors->Run(Motors::Location::Right, Motors::Direction::Forward);
    readLocation = LineSensors::Location::WideLeft;
    break;

  case Turning::Righty:
    mMotors->Run(Motors::Location::Left, Motors::Direction::Forward);
    mMotors->Run(Motors::Location::Right, Motors::Direction::Backward);
    readLocation = LineSensors::Location::MidLeft;
    break;

  case Turning::Righty1:
    mMotors->Run(Motors::Location::Left, Motors::Direction::Forward);
    mMotors->Run(Motors::Location::Right, Motors::Direction::Backward);
    readLocation = LineSensors::Location::WideLeft;
    break;

  case Turning::About:
    mMotors->Run(Motors::Location::Left, Motors::Direction::Forward);
    mMotors->Run(Motors::Location::Right, Motors::Direction::Backward);
    readLocation = LineSensors::Location::MidRight;
    break;
  
  default:
    break;
  }

  while (mLineSensors->Read(readLocation) != LineSensors::Background::White) {
    delay(10);
  }

  mMotors->Run(Motors::Location::Left, Motors::Direction::Forward);
  mMotors->Run(Motors::Location::Right, Motors::Direction::Forward);
}
