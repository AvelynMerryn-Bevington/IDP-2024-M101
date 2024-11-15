#ifndef M101_HARDWARE_CONFIG_H
#define M101_HARDWARE_CONFIG_H

#define PIN_ULTRASONIC_SENSOR A0
    
#define PIN_START_BUTTON 0
#define PIN_LEFT_MID_LINE_SENSOR 2
#define PIN_RIGHT_MID_LINE_SENSOR 3
#define PIN_LEFT_WIDE_LINE_SENSOR 4
#define PIN_RIGHT_WIDE_LINE_SENSOR 5

#define PIN_DRIVING_STATUS_LED 6
#define PIN_CONTAMINATION_LED 7
#define PIN_NO_CONTAMINATION_LED 8

#define PIN_SERVO_CLAW 9
#define PIN_SERVO_LIFT 10
#define PIN_MAGNETIC_SENSOR 11
#define PIN_TOUCH_SENSOR 12


#define MOTOR_LEFT 2
#define MOTOR_RIGHT 1

#define SAMPLE_RATE_IMU_HZ 10

#define ULTRASONIC_SENSOR_MAX_RANGE 520
#define ADC_ACCURACY 1023.0

#endif // M101_HARDWARE_CONFIG_H
