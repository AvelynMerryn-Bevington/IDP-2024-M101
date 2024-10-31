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

  enum Turning
  {
    Righty,
    Lefty,
    About,
    Lefty1,
    Righty1
  };

  void Turn(Turning direction);
  void FollowLine(int Slow, int Fast);
  bool CheckForJunction();
  void ChangingPurpose();


private:
  enum Purpose{
    CarryingBox,
    FetchingBox,
    ReturningToStart
  };

  int RouteCount = 0, BoxDeliveredCount = 0;
  bool Junction = false, ReadyForTurn = true, ContaminatedBox;
  std::array<Mapping::Direction, 10> Route;
  Robot::Purpose CurrentPurpose = FetchingBox;
  Mapping::Node CurrentDestination, CurrentLocation;

  void SetInitialSpeed();
  std::array<Mapping::Direction, 10> Robot::SelectingDestination(bool Contaminated);

  Motors *mMotors;
  Leds *mLeds;
  Imu *mImu;
  LineSensors *mLineSensors;
  Ultrasonic *mUltrasonic;
  Tof *mTof;
  StartButton *mStartButton;
  Mapping *mMapping;
  Claw *mClaw;
};

#endif // ROBOT_H
