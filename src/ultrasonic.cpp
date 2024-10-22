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

void Ultrasonic::UltrasonicDistance()
{
    float distance, direct_reading;

    direct_reading = analogRead(PIN_ULTRASONIC_SENSOR);
    distance = direct_reading * ULTRASONIC_SENSOR_MAX_RANGE / ADC_ACCURACY;

    Serial.print(distance,0);
    Serial.println("cm");

    
}
