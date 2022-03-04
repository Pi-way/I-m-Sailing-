/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       TurnTo.cpp                                                */
/*    Author:       Team 98548A                                               */
/*    Created:      8/20/2021                                                 */
/*    Description:  File that contains TurnTo() function                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

// Declare P, I, D, and RampUp values for this PID
float TTKp = .5;
float TTKi = .25;
float TTKd = 0.01;

// base function for TurnTo()
int _TurnTo_() {

  // Assign and declare local variables from global variables.
  float LocalTimeout = CoustomTimeout;
  float LocalTurnX = TurnX;
  float LocalTurnY = TurnY;
  float LocalMaxSpeed = Speed;
  float LocalTurnDegree = TurnDegree;

  // Update PIDsRunning
  PIDsRunning ++;

  // Wait until other PIDs have completed
  while(PIDsRunning > 1){
    task::yield();
  }

  float x1 = GpsX;
  float y1 = GpsY;
  float h1 = GpsH;
  float x2 = x1 + cos(Rads(h1));
  float y2 = y1 + sin(Rads(h1));
  float DistA = 1.0;
  float DistB = sqrt(powf(x1-LocalTurnX,2)+powf(y1-LocalTurnY,2));
  float DistC = sqrt(powf(x2-LocalTurnX,2)+powf(y2-LocalTurnY,2));
  float LocalTurn = Degs(acosf((powf(DistC,2)-powf(DistA,2)-powf(DistB,2))/(-2*DistA*DistB)));
  float MTX = LocalTurnX - x1;
  float MTY = LocalTurnY - y1;
  float RotatedY = -MTX*sin(Rads(h1))+MTY*cos(Rads(h1));
  LocalTurn = LocalTurn * -((RotatedY)/std::abs(RotatedY));

  SetDriveBrake(brake);
  SetDrivePosition(0);

  float ReachedTargetTime = 0;

  float Error = 0;
  float Integral = 0;
  float PreviousError = 0;
  float Derivative;
  float Voltage;
  float SmartError;
  int Ramp = 0;
  float ActualVoltage = 0;

  // This variable will determine wether or not the loop will continue to run
  bool NotDone = true;

  // Continue looping until done
  while (NotDone) {

    x1 = GpsX;
    y1 = GpsY;
    h1 = GpsH + LocalTurnDegree;
    x2 = x1 + cos(Rads(h1));
    y2 = y1 + sin(Rads(h1));
    DistB = sqrt(powf(x1-LocalTurnX,2)+powf(y1-LocalTurnY,2));
    DistC = sqrt(powf(x2-LocalTurnX,2)+powf(y2-LocalTurnY,2));
    LocalTurn = Degs(acosf((powf(DistC,2)-powf(DistA,2)-powf(DistB,2))/(-2*DistA*DistB)));
    MTX = LocalTurnX - x1;
    MTY = LocalTurnY - y1;
    RotatedY = -MTX*sin(Rads(h1))+MTY*cos(Rads(h1));
    LocalTurn = LocalTurn * -((RotatedY)/std::abs(RotatedY));

    Error = LocalTurn;
   
    Integral = Integral + Error;
    if(Integral > 40)(Integral = Error);
    Derivative = Error - PreviousError;
    if (Error == 0) {Integral = 0;} //these are to prevent the integral from getting too large
    if (std::abs(Voltage) > 5) {Integral = 0;}
    
    Ramp += 1;
    PreviousError = Error;

    SmartError = GetClosestToZero(Error, Ramp * (Error/std::abs(Error)));

    Voltage = SmartError * TTKp + Integral * TTKi + Derivative * TTKd;
    Voltage = GetClosestToZero(Voltage, LocalMaxSpeed * (Error/std::abs(Error)));

    if(std::abs(Voltage)<MinVoltage){
      Voltage = MinVoltage * (std::abs(Voltage)/Voltage);
    }

    if (ActualVoltage < Voltage){ActualVoltage = ActualVoltage + 1;}
    if (ActualVoltage > Voltage){ActualVoltage = ActualVoltage - 1;}

    FLMotor.spin(forward, ActualVoltage, voltageUnits::volt);
    FRMotor.spin(reverse, ActualVoltage, voltageUnits::volt);
    BRMotor.spin(reverse, ActualVoltage, voltageUnits::volt);
    BLMotor.spin(forward, ActualVoltage, voltageUnits::volt);

    // If the robot has run for longer than the LocalTimeout
    if (Brain.Timer.value() > LocalTimeout) { 
      NotDone = false;
    }
    
    // If the robot has gone past the target position
    if (std::abs(LocalTurn)-.5 <= 0) {

      // Record the moment when the robot first reaches the target position
      if (ReachedTargetTime == 0){
        ReachedTargetTime = ReachedTargetTime = Brain.Timer.systemHighResolution();
      }
    }

    // If the robot has been allowed to settle in for 1/4 of a second after the moment when the robot first reached the target
    if (Brain.Timer.systemHighResolution() - ReachedTargetTime > (0.25 * 1000000) && ReachedTargetTime != 0.0){
      NotDone = false;
    }

    task::yield();
  }
  

  Drivetrain(stop(coast);)

  PIDsRunning --;


  return 0;

}

void TurnTo(float Turn_x, float Turn_y, float speed, bool wait_for_completion, float Turn_Degree, float Coustom_Timeout) {

  Speed = (speed/100)*12;

  TurnDegree = Turn_Degree;
  CoustomTimeout = Coustom_Timeout * 1000000;
  TurnX = Turn_x;
  TurnY = Turn_y;

  if (wait_for_completion) {
    _TurnTo_();
  } else {
    PID = task(_TurnTo_);
  }

}