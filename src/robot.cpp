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
  //SEQUENCE TO REPEAT IF WIDE SENSORS NOT ACTIVATED
  FollowLine();


  //SEQUENCE TO READ IF WIDE SENSORS ACTIVATED
  bool RLS, LLS;
  if (mLineSensors->Read(LineSensors::Location::WideRight) == LineSensors::Background::White) {    RLS = true; } else {    RLS = false;  };
  if (mLineSensors->Read(LineSensors::Location::WideLeft) == LineSensors::Background::White) {    LLS = true; } else {    LLS = false;  };

  if (RLS == true || LLS == true) {

    mMotors->Run(Motors::Location::Left, Motors::Direction::Stopped);
    mMotors->Run(Motors::Location::Right, Motors::Direction::Stopped);
    //delay(500);


    if (count == 2) {
      mMotors->Turn(Motors::Turning::Righty);
    }
    if (count == 3 || count == 4 || count == 5 || count == 7 || count == 0 || count == 1 ) {
      //pass
    }
    if (count == 6 || count == 8 ) {
      mMotors->Turn(Motors::Turning::Lefty);
    }
    if (count == 9) {
      mMotors->Turn(Motors::Turning::Lefty1);
    }


    //END OF COMMANDS SEQUENCE
    bool online=true;
    while (online == true) {
      mMotors->Shuffle();
      if (mLineSensors->Read(LineSensors::Location::WideRight) == LineSensors::Background::Black && mLineSensors->Read(LineSensors::Location::WideLeft) == LineSensors::Background::Black) 
      {    online = false; } 
      else 
      {    online = true;  }    
    }

    if (RLS == true || LLS == true) {
      count += 1;
    }
    Serial.println(count);

    //------To perform AFTER robot comes off line (i.e. completes turn)

    if (count == 10) { //go forward and pickup box
      FollowLine();
      delay(300);
      FollowLine();
      delay(300);
      
      mMotors->Run(Motors::Location::Left, Motors::Direction::Stopped);
      mMotors->Run(Motors::Location::Right, Motors::Direction::Stopped);
      delay(500);
      mClaw->ServoPickup();
      
      Serial.println("SERVO PICKUP");

      mMotors->Turn(Motors::Turning::About);

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
  int Slow = 150, Fast = 200; //numbers that worked: 150 and 200


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

