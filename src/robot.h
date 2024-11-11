#ifndef ROBOT_H
#define ROBOT_H

#include "motors.h"
#include "leds.h"
#include "claw.h"
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

private:
  void Turn(Turning direction);
  void FollowLine(int Slow = 175, int Fast = 225);
  bool CheckForJunction();
  void ChangingPurpose();
  void ContaminatedDropoff();

  enum Purpose{
    CarryingBox,
    FetchingBox,
    ReturningToStart
  };

  const ::std::vector<Mapping::Node> DeliveryBays =
  {
    Mapping::Node::Box1Left,
    Mapping::Node::Box2Right,
    Mapping::Node::Box4Middle,
    Mapping::Node::Box1Right,
    Mapping::Node::Box4Middle,
    Mapping::Node::Box2Left, // Closest box, other than box 4, to the end.
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
  LineSensors *mLineSensors;
  Ultrasonic *mUltrasonic;
  Tof *mTof;
  StartButton *mStartButton;
  Mapping *mMapping;
  Claw *mClaw;
};

#endif // ROBOT_H
