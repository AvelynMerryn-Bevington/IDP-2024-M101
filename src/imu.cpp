#include "imu.h"
#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include "DFRobot_VL53L0X.h"

#include "m101_hardware_config.h"

Imu::Imu()
{
  return;

  Serial.print("LSM6DS3 IMU initialization ");

  if (!IMU.begin())
  {
    Serial.println("FAILED.");
    IMU.end();

    while(true){} // Kill the whole program
  }

  Serial.println("completed successfully.");
  Serial.println();

  madgwickFilter.begin(SAMPLE_RATE_IMU_HZ);
}

void Imu::GetYawAngle()
{
  return;

  char buffer[5];
  float ax, ay, az;
  float gx, gy, gz;
  if (!IMU.accelerationAvailable() ||
      !IMU.gyroscopeAvailable() ||
      !IMU.readAcceleration(ax, ay, az) ||
      !IMU.readGyroscope(gx, gy, gz))
  {
    return;
  }

  madgwickFilter.updateIMU(gx, gy, gz, ax, ay, az);
  Serial.print("Yaw = ");
  Serial.print(dtostrf(madgwickFilter.getYaw(), 4, 0, buffer));
  Serial.println(" °");
}
