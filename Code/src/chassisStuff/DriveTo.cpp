#include "vex.h"

float DKp = 100;
float DKi = 5;
float DKd = 0.1;

float GetClosestToZero(float First, float Second){
  if(std::abs(First) < std::abs(Second)){
    return First;
  }else{
    return Second;
  }
}

int _DriveTo_ (){

  //Pull global variables into Drive Function:
  float SessionTimeout = CoustomTimeout;
  float SessionDriveX = DriveX;
  float SessionDriveY = DriveY;
  float SessionMaxSpeed = Speed_V;
  float SessionDriveRadius;
  bool LimitSwitchFront;
  bool LimitSwitchBack;

  if (DriveRadius == -1.0){
    SessionDriveRadius = 0;
    LimitSwitchFront = true;
  }else{
    SessionDriveRadius = DriveRadius;
    LimitSwitchFront = false;
  }

  if (DriveRadius == -2.0){
    SessionDriveRadius = 0;
    LimitSwitchBack = true;
  }else{
    SessionDriveRadius = DriveRadius;
    LimitSwitchBack = false;
  }

  //Wait untill other PIDs are done to prevent conflicting motor commands:
  PIDsRunning ++;
  WaitUntil(PIDsRunning == 1);

  float StartTime = Brain.Timer.systemHighResolution();
  SetDriveBrake(hold);
  SetDrivePosition(0);

  float x1; float y1;
  float h1;                 //                         Variables for 
  float x2; float y2;       //                      law-of-cosines math
  float DistA = 1.0; float DistB;float DistC;
  float MTX; float MTY; 
  float RotatedY; //      Variables for rotated plane
  float SessionTurn;

  float PositiveDiagonalVoltage;
  float NegativeDiagonalVoltage;

  bool Condition = true;
  bool ReachedTarget = false;
  float ReachedTargetTime = 0;

  float Integral;
  float Derivative;
  float PreviousError;

  float Error;
  float Voltage;
  float SmartVoltage;
  float Ramp = 0;

  x1 = GpsX;
  y1 = GpsY;

  PreviousError = sqrtf(powf(x1-SessionDriveX, 2) + powf(y1-SessionDriveY, 2)) - SessionDriveRadius;

  while (Condition) {

    x1 = GpsX;
    y1 = GpsY;
    h1 = GpsH;
    x2 = x1 + cos(Rads(h1));
    y2 = y1 + sin(Rads(h1));
    DistB = sqrt(powf(x1-SessionDriveX,2)+powf(y1-SessionDriveY,2));                          //get Distance B for arccos function
    DistC = sqrt(powf(x2-SessionDriveX,2)+powf(y2-SessionDriveY,2));                          //get Distance C for arccos function
    SessionTurn = Degs(acosf((powf(DistC,2)-powf(DistA,2)-powf(DistB,2))/(-2*DistA*DistB)));  //get positive angle (from robot - to target)
    MTX = SessionDriveX - x1;                                                                 //move x1 to origin (kinda)
    MTY = SessionDriveY - y1;                                                                 //move y1 to origin
    RotatedY = -MTX*sin(Rads(h1))+MTY*cos(Rads(h1));                                          //get rotated Y value
    //SessionTurn = SessionTurn * -((RotatedY)/std::abs(RotatedY));                             //make SessionTurn the proper value

    SessionTurn = SessionTurn * ((RotatedY)/std::abs(RotatedY));                             //make SessionTurn the proper value

    Error = sqrtf(powf(x1-SessionDriveX, 2) + powf(y1-SessionDriveY, 2)) - SessionDriveRadius;

    if(LimitSwitchFront || LimitSwitchBack){
      if(Error < 5){
        Error = 5;//this keeps the pid moving for longer than usual when trying to grab a goal.
      }
    }
    SmartVoltage = GetClosestToZero(Error, Ramp * (Error/std::abs(Error)));                   //this is to keep wheels from slipping, ultimately resulting in a faster turn.

    Integral = Integral + Error;
    Derivative = Error - PreviousError;
    if (Error == 0) {Integral = 0;} //these are to prevent the integral from getting too large
    if (Error > 1) {Integral = 0;}
    if (std::abs(Error) > std::abs(SessionMaxSpeed/12)) {Integral = 0;}

    Ramp += 0.125;

    Voltage = SmartVoltage * DKp + Integral * DKi + Derivative * DKd;                                                                   //* TKp + Integral * TKi + Derivative * TKd;
    Voltage = GetClosestToZero(Voltage, SessionMaxSpeed * (Error/std::abs(Error)));

    if(std::abs(Voltage)<MinVoltage){
      Voltage = MinVoltage * (std::abs(Voltage)/Voltage);
    }

    PreviousError = Error;

    PositiveDiagonalVoltage = sin(Rads(SessionTurn+45))*std::abs(Voltage);
    NegativeDiagonalVoltage = -sin(Rads(SessionTurn-45))*std::abs(Voltage);

    FLMotor.spin(forward, NegativeDiagonalVoltage, voltageUnits::volt);
    FRMotor.spin(forward, PositiveDiagonalVoltage, voltageUnits::volt);
    BRMotor.spin(forward, NegativeDiagonalVoltage, voltageUnits::volt);
    BLMotor.spin(forward, PositiveDiagonalVoltage, voltageUnits::volt);

    if((LimitSwitchFront==true) && (FrontSensorsSenseATouch == true)){
      frontAir.set(true);
      Condition = false;
    } else if((LimitSwitchBack==true) && (LimSwitchBack.pressing())){
      backAir.set(true);
      Condition = false;
    }


    if (std::abs(Error)-4 <= 0.0 || ReachedTarget || Brain.Timer.systemHighResolution() - StartTime > SessionTimeout || ((LimitSwitchFront && FrontSensorsSenseATouch) || (LimitSwitchBack && LimSwitchBack.pressing()))) {
      if(!(ReachedTarget)){
        ReachedTargetTime = Brain.Timer.systemHighResolution();
      }

      ReachedTarget = true;

      if(Brain.Timer.systemHighResolution() - ReachedTargetTime >= 250000 || Brain.Timer.systemHighResolution() - StartTime > SessionTimeout){
        break;
      }
    }

    task::yield();

  }  

  Drivetrain(stop(brake);)

  PIDsRunning --;


  return 0;

}

void DriveTo(float Drive_x, float Drive_y, float speed, float radius, bool Wait_, float Coustom_Timeout){

  Speed_V = (speed/100)*12;

  DriveX = Drive_x;
  DriveY = Drive_y;
  CoustomTimeout = Coustom_Timeout * 1000000;

  DriveRadius = radius;

  Wait = Wait_;

  wait(20, msec);

  if (Wait) {

    _DriveTo_();

  } else {

    PID = task(_DriveTo_);

  }

  wait(20, msec);

}