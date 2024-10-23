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
 
    TofSensor.begin(0x50);
    TofSensor.setMode(TofSensor.eContinuous,TofSensor.eHigh);
    TofSensor.start();
}

void Tof::TofDistance()
{   
    float distance = TofSensor.getDistance();
    Serial.print("TOF Distance: ");
    Serial.println(distance);
 
}