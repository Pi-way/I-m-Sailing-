/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       TurnTo.cpp                                                */
/*    Author:       Team 98548A                                               */
/*    Created:      8/20/2021                                                 */
/*    Description:  File that contains TurnTo() function                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

float TTKp = .5;
float TTKi = .25;
float TTKd = 0.01;

int _TurnTo_() {

  float LocalTimeout = CoustomTimeout;
  float LocalTurnX = TurnX;
  float LocalTurnY = TurnY;
  float LocalMaxSpeed = Speed;
  float LocalTurnDegree = TurnDegree;

  PIDsRunning ++;

  while(PIDsRunning > 1){
      task::yield();
  }

  float StartTime = Brain.Timer.systemHighResolution();

  float AverageX = 0;
  float AverageY = 0;
  float AverageH = 0;

  int SampleSize = 0;

  while(Brain.Timer.systemHighResolution() - StartTime < 1000000){
    AverageX += GpsX;
    AverageY += GpsY;
    AverageH += GpsH + LocalTurnDegree;
    SampleSize += 1;
    task::yield();
  }

  float x1 = AverageX / SampleSize;
  float y1 = AverageY / SampleSize;
  float h1 = AverageH / SampleSize;

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

  Brain.Screen.print(LocalTurn);
  Brain.Screen.setCursor(2,1);

  SetDriveBrake(brake);

  SetDrivePosition(0);

  bool Condition = true;
  bool ReachedTarget = false;

  float ReachedTargetTime = 0;

  float Error = 0;
  float Integral = 0;
  float PreviousError = 0;
  float Derivative;
  float Voltage;
  float SmartError;
  int Ramp = 0;
  float ActualVoltage = 0;


  while (Condition) {

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

    if (std::abs(LocalTurn)-.5 <= 0 || ReachedTarget || Brain.Timer.systemHighResolution() - StartTime > LocalTimeout) {
      if(!(ReachedTarget)){
        ReachedTargetTime = Brain.Timer.systemHighResolution();
      }

      ReachedTarget = true;

      if(Brain.Timer.systemHighResolution() - ReachedTargetTime >= 250000){
        break;
      }
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