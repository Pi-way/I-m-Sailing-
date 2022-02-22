#include "vex.h"

float TKp = .5;
float TKi = .25;
float TKd = 0.01;

int _Turn_() {


  /*
  turns the robot in degrees
  */
  float SessionTimeout = CoustomTimeout;
  float SessionTurn = TurnDistance;
  float SessionSpeed = Speed_V;

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

  Inertial.setRotation(0, deg);

  Brain.Timer.reset();

  SetDriveBrake(coast);

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
  while (Condition) {

    //avgm = std::abs((FrontLeftDrive.position(degrees) - FrontRightDrive.position(degrees) + BackLeftDrive.position(degrees) -     BackRightDrive.position(degrees)) / 4);
    avgm = std::abs(Inertial.rotation(degrees));

    Error = std::abs(SessionTurn) - avgm;
    Integral = Integral + Error;

    if (Error <= x) {
      smartError = Error;
    } else if (x <= Error) {
       smartError = x;
     }

    if (Error == 0) {
      Integral = 0;
    }

    if (std::abs(Error) > SessionSpeed/12 ) {
      Integral = 0;
    }

    Derivative = Error - PreviousError;
    PreviousError = Error;

    Speed = smartError * TKp + Integral * TKi + Derivative * TKd;

    x += 2;

    if (Speed > SessionSpeed) {
      Speed = SessionSpeed;
    }

    if (SessionTurn > 0) {
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

    if (std::abs(avgm) > std::abs(SessionTurn) + 3.5 || Brain.Timer.value() > SessionTimeout) { //was 40

      Condition = false;

    }

    if (SessionWait) {
      wait(20, msec);
    } else {
      task::yield();
    }

  }
  

  FLMotor.stop();
  FRMotor.stop();
  BRMotor.stop();
  BLMotor.stop();

  PIDsRunning --;


  return 0;

}

void Turn(float Turn_, float speed, bool Wait_, float CoustomTimeout_) {

  Speed_V = (speed/100)*12;

  CoustomTimeout = CoustomTimeout_;
  TurnDistance = Turn_;
  Wait = Wait_;

  wait(20, msec);

  if (Wait) {

    _Turn_();

  } else {

    TURN_PID = task(_Turn_);

  }

  wait(20, msec);

}