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

float Ultrasonic::UltrasonicDistance()
{
    float distance, direct_reading;

    direct_reading = analogRead(PIN_ULTRASONIC_SENSOR);
    distance = direct_reading * ULTRASONIC_SENSOR_MAX_RANGE / ADC_ACCURACY;

    Serial.print(distance,0);
    Serial.println("cm");

    return distance;
}

bool Ultrasonic::UltrasonicBoxCheck()
//Reads the average of 20 readings of the ultrasonic sensor over 2 seconds
// if the average distance is less than x, it will return boolean true.
{
    float distance, sum = 0, i;

    for (i=0; i<20; i++) {
        distance = UltrasonicDistance();
        sum += distance;
        delay(100); //adjust values after testing
    }
    sum = distance/50;

    if (sum <= 2.5 && sum > 0.0) { //adjust values after testing
        return true;
    } else if (sum > 2.5) {
        return false;
    } else {
        Serial.println("ULTRASONIC READING ERROR");
        return false;
    }

}
