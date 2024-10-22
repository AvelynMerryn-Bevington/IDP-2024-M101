#ifndef ROBOT_H
#define ROBOT_H

#include "motors.h"
#include "leds.h"
#include "claw.h"
#include "imu.h"
#include "line_sensors.h"
#include "ultrasonic.h"
#include "start_button.h"


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
  Imu *mImu;
  LineSensors *mLineSensors;
  Ultrasonic *mUltrasonic;
  StartButton *mStartButton;
};

#endif // ROBOT_H
