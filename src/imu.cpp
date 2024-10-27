#include "imu.h"
#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include "DFRobot_VL53L0X.h"

#include "m101_hardware_config.h"

Imu::Imu()
{
  Serial.print("LSM6DS3 IMU initialization ");
  Serial.flush();
  
  if (!IMU.begin())
  {
    Serial.println("FAILED.");
    Serial.flush();

    IMU.end();
    while(true){} // Kill the whole program
  }
  mMadgwick.begin(SAMPLE_RATE_IMU_HZ);

  Serial.println("completed successfully.");
  Serial.flush();
}


//Kerry: who wrote this function? Are we going to use it? ----> To do: write the output code, rather than to Serial
void Imu::GetYawAngle()
{
  float ax, ay, az;
  float gx, gy, gz;
  if (IMU.accelerationAvailable()
      && IMU.gyroscopeAvailable()
      && IMU.readAcceleration(ax, ay, az)
      && IMU.readGyroscope(gx, gy, gz))
  {
    mMadgwick.updateIMU(gx, gy, gz, ax, ay, az);
    Serial.print("Yaw = ");
    Serial.print(mMadgwick.getYaw());
    Serial.println(" °");
  }
}
