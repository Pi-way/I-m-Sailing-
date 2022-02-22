#include "vex.h"

float fKp = 0.05;
float fKi = 0.01;
float fKd = 0.1;

float Drive_balance = -0.045;

int _Drive_Fast() {
  float SessionDistance = ((Distance/12.566))*360;
  bool SessionWait = Wait;
  float SessionSpeed_V = Speed_V;
  bool SessionCoast = Coast;
  float SessionMaxDistance = MaxDistance;

  bool FrontButton = fb;
  bool BackButton = bb;

  PIDsRunning ++;

  while(PIDsRunning > 1){
    task::yield();
  }

  Brain.Timer.reset();

  SetDriveBrake(hold);

  FLMotor.setPosition(0, degrees);
  FRMotor.setPosition(0, degrees);
  BLMotor.setPosition(0, degrees);
  BRMotor.setPosition(0, degrees);

  bool Condition = true;

  float Error;
  float Integral = 0;
  float PreviousError = 0;
  float Derivative;
  float Speed;
  float smartError;
  int x = 0;
  float avgm;

  float StartEndTime = 0.0;

  bool STAHP = false;

  while (Condition) {

    avgm = std::abs((FLMotor.position(degrees) + FRMotor.position(degrees) + BLMotor.position(degrees) + BRMotor.position(degrees)) / 4);

    Error = std::abs(SessionDistance) - avgm;

    if(Error < ((5/12.566))*360 && (FrontButton || BackButton)){
      if (avgm > ((SessionMaxDistance/12.566))*360){
        STAHP = true;
      }else{
        Error = ((5/12.566))*360;
      }
    }

    Integral = Integral + Error;

    if (Error <= x) {
      smartError = Error;
    } else if (x <= Error) {
       smartError = x;
    }
 

    if (Error == 0) {
      Integral = 0;
    }

    if (std::abs(Error) > SessionSpeed_V ) {
      Integral = 0;
    }

    Derivative = Error - PreviousError;
    PreviousError = Error;

    Speed = smartError * fKp + Integral * fKi + Derivative * fKd;

    x += 60;

    if (Speed > SessionSpeed_V) {
      Speed = SessionSpeed_V;
    }

    if (SessionDistance > 0) {
    FLMotor.spin(forward, Speed + Speed * Drive_balance, voltageUnits::volt);
    FRMotor.spin(forward, Speed - Speed * Drive_balance, voltageUnits::volt);
    BRMotor.spin(forward, Speed - Speed * Drive_balance, voltageUnits::volt);
    BLMotor.spin(forward, Speed + Speed * Drive_balance, voltageUnits::volt);
    } else {
      FLMotor.spin(reverse, Speed + Speed * Drive_balance, voltageUnits::volt);
      FRMotor.spin(reverse, Speed - Speed * Drive_balance, voltageUnits::volt);
      BRMotor.spin(reverse, Speed - Speed * Drive_balance, voltageUnits::volt);
      BLMotor.spin(reverse, Speed + Speed * Drive_balance, voltageUnits::volt);
    }

    if ((avgm > std::abs(SessionDistance) && !(FrontButton || BackButton)) || Brain.Timer.value() > 3 || ((FrontButton && FrontSensorsSenseATouch)||(BackButton && LimSwitchBack.pressing())) || STAHP) {    //Was: avgm > std::abs(SessionDistance) - 40 || Brain.Timer.value() > 4

      if (StartEndTime == 0.0){
        StartEndTime = Brain.Timer.systemHighResolution();
      }

      if (FrontButton || BackButton || STAHP) {
        if(Brain.Timer.systemHighResolution() - StartEndTime > 250000 || Brain.Timer.value() > 3){
          Condition = false;
        }
        if (STAHP){
          Condition = false;
        }
      } else {
        Condition = false;
      }
    }

    if (SessionWait) {
      wait(20, msec);
    } else {
      task::yield();
    }

  }

  if(SessionCoast){
    Drivetrain(stop(hold););
  }else{
    Drivetrain(stop(hold););
  }

  PIDsRunning --;

  return 0;

}

void DriveFast(float Distance_, float Speed_V_, bool Wait_, bool f_b, bool b_b, bool coast, float maxDistance) {

  Distance = Distance_;
  Wait = Wait_;
  Speed_V = (Speed_V_/100)*12;
  fb= f_b;
  bb = b_b;
  Coast = coast;
  MaxDistance = maxDistance;

  wait(20, msec);

  if (Wait) {

    _Drive_Fast();

  } else {

    PID = task(_Drive_Fast);

  }

  wait(20, msec);

}