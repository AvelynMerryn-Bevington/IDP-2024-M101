#ifndef ROBOT_H
#define ROBOT_H

#include "motors.h"
#include "leds.h"
#include "claw.h"
#include "imu.h"
#include "line_sensors.h"
#include "ultrasonic.h"
#include "tof.h"
#include "start_button.h"
#include "mapping.h"
#include "ArxContainer.h"

class Robot
{
public:
  void Init();
  void Loop();

  

private:
  enum Purpose{
    CarryingBox,
    FetchingBox,
    ReturningToStart
  };

  int RouteCount = 0, BoxDeliveredCount = 0;
  bool Junction = true, ReadyForTurn = true;
  std::array<Mapping::Direction, 10> Route;
  Robot::Purpose CurrentPurpose = FetchingBox;
  Mapping::Node CurrentDestination, CurrentLocation;

  void SetInitialSpeed();
  void FollowLine();
  bool CheckForJunction();
  std::array<Mapping::Direction, 10> Robot::SelectingDestination(bool Contaminated);

  Motors *mMotors;
  Leds *mLeds;
  Claw *mClaw;
  Imu *mImu;
  LineSensors *mLineSensors;
  Ultrasonic *mUltrasonic;
  Tof *mTof;
  StartButton *mStartButton;
  Mapping *mMapping;
};

#endif // ROBOT_H
