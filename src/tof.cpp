#include "tof.h"
#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include "DFRobot_VL53L0X.h"
#include "Wire.h"

#include "m101_hardware_config.h"

Tof::Tof()
{
  Serial.print("TOF Sensor Setup...");
  Wire.begin();
 
  mTofSensor.begin(0x50);
  mTofSensor.setMode(mTofSensor.eContinuous, mTofSensor.eHigh);
  mTofSensor.start();
  Serial.println("Done!");
}

float Tof::TofDistance()
{   
  const float Distance = mTofSensor.getDistance() / 10;
  return(Distance);
}

bool Tof::ContaminationBayDrop()
{
    float sum = 0;
    const int ReadingCount = 20;
    for (int i = 0; i < ReadingCount; i++)
    {
        sum += TofDistance();
    }
    const float Average = sum / ReadingCount;

    if (Average < 0.0)
    {
        Serial.println("ULTRASONIC READING ERROR");
        return false;
    }
    Serial.println(Average);

    const float CutoffDistance = 6;
    return (Average <= CutoffDistance);
}