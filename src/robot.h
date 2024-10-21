#ifndef ROBOT_H
#define ROBOT_H

#include "motors.h"
#include "leds.h"
#include "claw.h"
#include "imu.h"
#include "line_sensors.h"

class Robot
{
public:
  void Init();
  void Loop();

private:
  void SetInitialSpeed();
  void FollowLine();

  Motors *mMotors;
  Leds *mLeds;
  Claw *mClaw;
//  Imu *mImu;
  LineSensors *mLineSensors;
};

#endif // ROBOT_H
