#ifndef IMU_H
#define IMU_H

#include <MadgwickAHRS.h>

class Imu
{
public:
  Imu();
  void GetYawAngle();

private:
  Madgwick madgwickFilter;
};

#endif // IMU_H
