
#ifndef TOF_H
#define TOF_H

#include "DFRobot_VL53L0X.h"

class Tof
{
public:
  Tof();
  bool ContaminationBayDrop();

private:
  float TofDistance();
  DFRobot_VL53L0X mTofSensor;
};

#endif // TOF_H
