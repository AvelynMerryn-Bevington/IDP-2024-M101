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

void Tof::TofDistance()
{   
  const float Distance = mTofSensor.getDistance() / 10;
  Serial.print("TOF Distance: ");
  Serial.println(Distance);
}
