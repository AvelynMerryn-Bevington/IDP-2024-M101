#include "motors.h"
#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include "DFRobot_VL53L0X.h"
#include "line_sensors.h"

#include "m101_hardware_config.h"

Motors::Motors()
{
  Serial.print("Motors setup...");
  Serial.flush();
  mMotorShield = Adafruit_MotorShield();

  if (!mMotorShield.begin())
  {
    Serial.println("Could not find Motor Shield. Check wiring.");
    Serial.flush();
    while(true){} // Kill the whole entire program
  }

  Serial.println("Done!");
  Serial.flush();
}

Adafruit_DCMotor* Motors::GetMotor(Location loc)
{
  switch (loc)
  {
  case Location::Left:
    return mMotorShield.getMotor(MOTOR_LEFT);
  
  case Location::Right:
    return mMotorShield.getMotor(MOTOR_RIGHT);

  default:
    return NULL;
  }
}

void Motors::Run(const Location loc, const Direction direction)
{
  Adafruit_DCMotor *motor = GetMotor(loc);

  switch (direction)
  {
  case Forward:
    motor->run(FORWARD);
    break;

  case Backward:
    motor->run(BACKWARD);
    break;

  case Stopped:
    motor->run(RELEASE);
    break;
  }
}

void Motors::SetSpeed(const Location loc, const uint8_t speed)
//input (which motor? , speed)
//sets the speed of the motor. Use mmotor->run() command to activate motor
{
  Adafruit_DCMotor *motor = GetMotor(loc);

  mMotorSpeeds[loc] = speed;
  motor->setSpeed(speed);
}

void Motors::AdjustSpeed(const Location loc, const int speedChange)
{
  uint8_t newSpeed = static_cast<uint8_t>(static_cast<int>(mMotorSpeeds[loc]) + speedChange);
  SetSpeed(loc, newSpeed);
}

void Motors::Turn(Turning direction)
//Input: Turn direction (Lefty/Righty)
//No code output. Makes robot turn 90 degrees in either direction
//After turn, make sure to reengage linefollowing algorithm
{
  Adafruit_DCMotor *motorleft = GetMotor(Left);
  Adafruit_DCMotor *motorright = GetMotor(Right);

  uint8_t i;

  long buffer_period = 500, buffer_speed = 200, iterating_speed = 200;

  //stop motors
  motorleft->run(RELEASE);
  motorright->run(RELEASE);
  delay(100);

  if (direction == Lefty) //left turn
  {
    motorleft->run(BACKWARD);
    motorright->run(FORWARD);

    //rotate for little bit (so the line sensors can leave the white line without complications)
    motorleft->setSpeed(buffer_speed);
    motorright->setSpeed(buffer_speed);
    delay(buffer_period);

    //turn until line sensor detects line
    bool rotate = true;
    while (rotate == true) {
      //turn
      motorleft->setSpeed(iterating_speed);
      motorright->setSpeed(iterating_speed);
      delay(10);

      //Test to see if line is detected
      if (mLineSensors->Read(LineSensors::Location::MidRight) == LineSensors::Background::White) {
        rotate = false;
      } else {
        rotate = true;
      }
    }
    //extra delay to get the car parallel with line
    motorleft->run(FORWARD);
    motorright->run(FORWARD);

  } 
  else if (direction == Righty) //right turn
  {
    motorleft->run(FORWARD);
    motorright->run(BACKWARD);

    //rotate for little bit (so the line sensors can leave the white line without complications)
    motorleft->setSpeed(buffer_speed);
    motorright->setSpeed(buffer_speed);
    delay(buffer_period);

    //turn until line sensor detects line
    bool rotate = true;
    while (rotate == true) {
      //turn
      motorleft->setSpeed(iterating_speed);
      motorright->setSpeed(iterating_speed);
      delay(10);

      //Test to see if line is detected
      if (mLineSensors->Read(LineSensors::Location::MidLeft) == LineSensors::Background::White) {
        rotate = false;
      } else {
        rotate = true;
      }
    }
    //extra delay to get the car parallel with line
    motorleft->run(FORWARD);
    motorright->run(FORWARD);

  }
  
  else if (direction == About) //about turn (180 deg) TO THE LEFT

  // u turn ONLY WORKS if there are NO OTHER LINES (ie 90deg turn lines) to interrupt it -> do not turn at nodes w 90deg turns
  // (it is the same code as a left turn)
  {
    motorleft->run(BACKWARD);
    motorright->run(FORWARD);

    //rotate for little bit (so the line sensors can leave the white line without complications)
    motorleft->setSpeed(buffer_speed);
    motorright->setSpeed(buffer_speed);
    delay(buffer_period);

    //turn until line sensor detects line
    bool rotate = true;
    while (rotate == true) {
      //turn
      motorleft->setSpeed(iterating_speed);
      motorright->setSpeed(iterating_speed);
      delay(10);

      //Test to see if line is detected
      if (mLineSensors->Read(LineSensors::Location::MidRight) == LineSensors::Background::White) {
        rotate = false;
      } else {
        rotate = true;
      }
      
    }
    motorleft->run(FORWARD);
    motorright->run(FORWARD);

  } else if (direction == Lefty1) //left turn with straight line ahead
  {
    motorleft->run(BACKWARD);
    motorright->run(FORWARD);

    //rotate for little bit (so the line sensors can leave the white line without complications)
    motorleft->setSpeed(buffer_speed);
    motorright->setSpeed(buffer_speed);
    delay(buffer_period);

    //turn until line sensor detects line
    bool rotate = true;
    while (rotate == true) {
      //turn
      motorleft->setSpeed(iterating_speed);
      motorright->setSpeed(iterating_speed);
      delay(10);

      //Test to see if line is detected
      if (mLineSensors->Read(LineSensors::Location::WideLeft) == LineSensors::Background::White) {
        rotate = false;
      } else {
        rotate = true;
      }
    }
    //extra delay to get the car parallel with line
    motorleft->run(FORWARD);
    motorright->run(FORWARD);

  } else if (direction == Righty1) //left turn with straight line ahead
  {
    motorleft->run(FORWARD);
    motorright->run(BACKWARD);

    //rotate for little bit (so the line sensors can leave the white line without complications)
    motorleft->setSpeed(buffer_speed);
    motorright->setSpeed(buffer_speed);
    delay(buffer_period);

    //turn until line sensor detects line
    bool rotate = true;
    while (rotate == true) {
      //turn
      motorleft->setSpeed(iterating_speed);
      motorright->setSpeed(iterating_speed);
      delay(10);

      //Test to see if line is detected
      if (mLineSensors->Read(LineSensors::Location::WideLeft) == LineSensors::Background::White) {
        rotate = false;
      } else {
        rotate = true;
      }
    }
    //extra delay to get the car parallel with line
    motorleft->run(FORWARD);
    motorright->run(FORWARD);

  } 
  //Consider writing in the followline() code into the end of the turn function -> speak to Kerry

}

void Motors::Shuffle()
{
  Adafruit_DCMotor *motorleft = GetMotor(Left);
  Adafruit_DCMotor *motorright = GetMotor(Right);

  uint8_t i;

  //stop motors
  //motorleft->run(RELEASE);
  //motorright->run(RELEASE);
  //delay(100);

  motorleft->run(FORWARD); //-----CHANGE TO FORWARD
  motorright->run(FORWARD);

  motorleft->setSpeed(200);
  motorright->setSpeed(200);

}








