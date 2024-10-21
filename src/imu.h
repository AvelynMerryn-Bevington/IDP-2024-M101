#ifndef IMU_H
#define IMU_H

#include <MadgwickAHRS.h>

class Imu
{
public:
  Imu();
  void GetYawAngle();

private:
  Madgwick mMadgwick;
};

#endif // IMU_H
