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
  void FollowLine(int Slow = 150, int Fast = 200);
  bool CheckForJunction();
  void ChangingPurpose();


private:
  enum Purpose{
    CarryingBox,
    FetchingBox,
    ReturningToStart
  };

  const ::std::vector<Mapping::Node> DeliveryBays =
  {
    Mapping::Node::Box1Left,
    Mapping::Node::Box1Right,
    Mapping::Node::Box2Left,
    Mapping::Node::Box2Right,
    Mapping::Node::Box3Left,
    Mapping::Node::Box3Right,
    Mapping::Node::Box4Middle
  };
  ::std::vector<Mapping::Node> mDeliveredBoxes;

  bool mJunction = false;
  bool mReadyForTurn = true;
  bool mContaminatedBox = false;
  ::std::vector<Mapping::Direction> mRoute;
  Robot::Purpose mCurrentPurpose = FetchingBox;
  Mapping::Node mCurrentDestination;
  Mapping::Node mCurrentLocation;

  Mapping::Node GetNextDeliveryNode();
  void SetInitialSpeed();
  ::std::vector<Mapping::Direction> Robot::SelectingDestination(bool Contaminated);

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
