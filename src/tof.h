
#ifndef TOF_H
#define TOF_H

#include "DFRobot_VL53L0X.h"


class Tof
{
public:
    Tof();
    void TofDistance();
private:
    DFRobot_VL53L0X TofSensor;

};
#endif // TOF_H