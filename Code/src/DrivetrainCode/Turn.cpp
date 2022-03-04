/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       Turn.cpp                                                  */
/*    Author:       Team 98548A                                               */
/*    Created:      8/20/2021                                                 */
/*    Description:  File that contains Turn() function                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

float TKp = .5;
float TKi = .25;
float TKd = 0.01;

int _Turn_() {

  float LocalTimeout = CoustomTimeout;
  float LocalTurn = TurnDistance;
  float LocalSpeed = Speed;

  PIDsRunning ++;

  while(PIDsRunning > 1){
    task::yield();
  }

  Inertial.setRotation(0, deg);

  Brain.Timer.reset();

  SetDriveBrake(coast);

  SetDrivePosition(0);

  bool Condition = true;

  float Error;
  float Integral = 0;
  float PreviousError = 0;
  float Derivative;
  float Speed;
  float smartError;
  int x = 0;
  float avgm;
  while (Condition) {

    //avgm = std::abs((FrontLeftDrive.position(degrees) - FrontRightDrive.position(degrees) + BackLeftDrive.position(degrees) -     BackRightDrive.position(degrees)) / 4);
    avgm = std::abs(Inertial.rotation(degrees));

    Error = std::abs(LocalTurn) - avgm;
    Integral = Integral + Error;

    smartError = GetClosestToZero(x, Error);

    if (Error == 0) {
      Integral = 0;
    }

    if (std::abs(Error) > LocalSpeed/12 ) {
      Integral = 0;
    }

    Derivative = Error - PreviousError;
    PreviousError = Error;

    Speed = smartError * TKp + Integral * TKi + Derivative * TKd;

    x += 2;

    if (Speed > LocalSpeed) {
      Speed = LocalSpeed;
    }

    if (LocalTurn > 0) {
    FLMotor.spin(forward, Speed, voltageUnits::volt);
    FRMotor.spin(reverse, Speed, voltageUnits::volt);
    BRMotor.spin(reverse, Speed, voltageUnits::volt);
    BLMotor.spin(forward, Speed, voltageUnits::volt);
    } else {
      FLMotor.spin(reverse, Speed, voltageUnits::volt);
      FRMotor.spin(forward, Speed, voltageUnits::volt);
      BRMotor.spin(forward, Speed, voltageUnits::volt);
      BLMotor.spin(reverse, Speed, voltageUnits::volt);
    }

    if (std::abs(avgm) > std::abs(LocalTurn) + 3.5 || Brain.Timer.value() > LocalTimeout) { //was 40

      Condition = false;

    }

      task::yield();

  }
  
  Drivetrain(stop(coast);)

  PIDsRunning --;

  return 0;

}

void Turn(float Turn_, float speed, bool wait_for_completion, float CoustomTimeout_) {

  Speed = (speed/100)*12;

  CoustomTimeout = CoustomTimeout_;
  TurnDistance = Turn_;

  if (wait_for_completion) {
    _Turn_();
  } else {
    PID = task(_Turn_);
  }

}