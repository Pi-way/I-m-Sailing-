#include "vex.h"

float TTKp = .5;
float TTKi = .25;
float TTKd = 0.01;

int _TurnTo_() {

  /*
  turns the robot in degrees
  */

  float SessionTimeout = CoustomTimeout;
  float SessionTurnX = TurnX;
  float SessionTurnY = TurnY;
  float SessionMaxSpeed = Speed;
  float SessionTurnDegree = TurnDegree;

  //float SessionTurn = ((( DriveCircumfrence * ( TurnDistance / 360 )) / ( WheelDiameter * 3.14 )) / DriveGearRatio ) *360;
  bool SessionWait = Wait;

  PIDsRunning ++;

  while(PIDsRunning > 1){
    if (SessionWait) {
      wait(20, msec);
    } else {
      task::yield();
    }
  }

  float StartTime = Brain.Timer.systemHighResolution();

  float AverageX = 0;
  float AverageY = 0;
  float AverageH = 0;

  int SampleSize = 0;

  while(Brain.Timer.systemHighResolution() - StartTime < 1000000){
    AverageX += GpsX;
    AverageY += GpsY;
    AverageH += GpsH + SessionTurnDegree;
    SampleSize += 1;
    task::yield();
  }

  float x1 = AverageX / SampleSize;
  float y1 = AverageY / SampleSize;
  float h1 = AverageH / SampleSize;

  float x2 = x1 + cos(Rads(h1));
  float y2 = y1 + sin(Rads(h1));

  float DistA = 1.0;
  float DistB = sqrt(powf(x1-SessionTurnX,2)+powf(y1-SessionTurnY,2));
  float DistC = sqrt(powf(x2-SessionTurnX,2)+powf(y2-SessionTurnY,2));

  float SessionTurn = Degs(acosf((powf(DistC,2)-powf(DistA,2)-powf(DistB,2))/(-2*DistA*DistB)));

  float MTX = SessionTurnX - x1;
  float MTY = SessionTurnY - y1;

  float RotatedY = -MTX*sin(Rads(h1))+MTY*cos(Rads(h1));

  SessionTurn = SessionTurn * -((RotatedY)/std::abs(RotatedY));

  Brain.Screen.print(SessionTurn);
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
    h1 = GpsH + SessionTurnDegree;
    x2 = x1 + cos(Rads(h1));
    y2 = y1 + sin(Rads(h1));
    DistB = sqrt(powf(x1-SessionTurnX,2)+powf(y1-SessionTurnY,2));
    DistC = sqrt(powf(x2-SessionTurnX,2)+powf(y2-SessionTurnY,2));
    SessionTurn = Degs(acosf((powf(DistC,2)-powf(DistA,2)-powf(DistB,2))/(-2*DistA*DistB)));
    MTX = SessionTurnX - x1;
    MTY = SessionTurnY - y1;
    RotatedY = -MTX*sin(Rads(h1))+MTY*cos(Rads(h1));
    SessionTurn = SessionTurn * -((RotatedY)/std::abs(RotatedY));

    Error = SessionTurn;
    // Error = (1/(1+ powf(2.718, -(SessionTurn)/9.5))) * (SessionMaxSpeed*2) - SessionMaxSpeed;

    Integral = Integral + Error;
    if(Integral > 40)(Integral = Error);
    Derivative = Error - PreviousError;
    if (Error == 0) {Integral = 0;} //these are to prevent the integral from getting too large
    if (std::abs(Voltage) > 5) {Integral = 0;}
    //if (std::abs(Error) > std::abs(SessionMaxSpeed/12)) {Integral = 0;}

    Ramp += 1;
    PreviousError = Error;

    SmartError = GetClosestToZero(Error, Ramp * (Error/std::abs(Error)));

    Voltage = SmartError * TTKp + Integral * TTKi + Derivative * TTKd;
    Voltage = GetClosestToZero(Voltage, SessionMaxSpeed * (Error/std::abs(Error)));

    if(std::abs(Voltage)<MinVoltage){
      Voltage = MinVoltage * (std::abs(Voltage)/Voltage);
    }

    if (ActualVoltage < Voltage){ActualVoltage = ActualVoltage + 1;}
    if (ActualVoltage > Voltage){ActualVoltage = ActualVoltage - 1;}

    FLMotor.spin(forward, ActualVoltage, voltageUnits::volt);
    FRMotor.spin(reverse, ActualVoltage, voltageUnits::volt);
    BRMotor.spin(reverse, ActualVoltage, voltageUnits::volt);
    BLMotor.spin(forward, ActualVoltage, voltageUnits::volt);

    if (std::abs(SessionTurn)-.5 <= 0 || ReachedTarget || Brain.Timer.systemHighResolution() - StartTime > SessionTimeout) {
      if(!(ReachedTarget)){
        ReachedTargetTime = Brain.Timer.systemHighResolution();
      }

      ReachedTarget = true;

      if(Brain.Timer.systemHighResolution() - ReachedTargetTime >= 250000){
        break;
      }
    }

    if (SessionWait) {
      wait(20, msec);
    } else {
      task::yield();
    }

  }
  

  Drivetrain(stop(coast);)

  PIDsRunning --;


  return 0;

}

void TurnTo(float Turn_x, float Turn_y, float speed, bool Wait_, float Turn_Degree, float Coustom_Timeout) {

  Speed = (speed/100)*12;

  TurnDegree = Turn_Degree;
  CoustomTimeout = Coustom_Timeout * 1000000;
  TurnX = Turn_x;
  TurnY = Turn_y;
  Wait = Wait_;

  wait(20, msec);

  if (Wait) {

    _TurnTo_();

  } else {

    PID = task(_TurnTo_);

  }

  wait(20, msec);

}