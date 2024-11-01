#include "ultrasonic.h"
#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include "DFRobot_VL53L0X.h"

#include "m101_hardware_config.h"

Ultrasonic::Ultrasonic()
{
    Serial.print("Ultrasonic Sensor Setup...");
    pinMode(PIN_ULTRASONIC_SENSOR, INPUT);
}

float Ultrasonic::Distance()
{
    float distance, direct_reading;

    direct_reading = analogRead(PIN_ULTRASONIC_SENSOR);
    distance = direct_reading * ULTRASONIC_SENSOR_MAX_RANGE / ADC_ACCURACY;

    return distance;
}

bool Ultrasonic::BoxCheck()
{
    float sum = 0;
    const int ReadingCount = 100;
    for (int i = 0; i < ReadingCount; i++)
    {
        sum += Distance();
    }
    const float Average = sum / ReadingCount;

    if (Average < 0.0)
    {
        Serial.println("ULTRASONIC READING ERROR");
        return false;
    }
    Serial.println(Average);

    const float CutoffDistance = 1.0;
    return (Average <= CutoffDistance);
}
