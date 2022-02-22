#include "vex.h"

void Calibrate(){
  Calibrated = false;
  PIDsRunning = 0;
  FrontSensorsSenseATouch = false;

  BRMotor.spin(forward);
  BLMotor.spin(forward);
  FRMotor.spin(forward);
  FLMotor.spin(forward);
  BackLift.spin(forward);
  FrontLift.spin(forward);

  RingLiftL.setStopping(coast);
  RingLiftR.setStopping(coast);

  BRMotor.setVelocity(0,percent);
  BLMotor.setVelocity(0,percent);
  FRMotor.setVelocity(0,percent);
  FLMotor.setVelocity(0,percent);
  BackLift.setVelocity(0, percent);
  FrontLift.setVelocity(0, percent);

  BRMotor.setStopping(coast);
  BLMotor.setStopping(coast);
  FRMotor.setStopping(coast);
  FLMotor.setStopping(coast);
  BackLift.setStopping(hold);
  FrontLift.setStopping(hold);

  if(Calibrated){

    return;

  }

  BackLift.setPosition(0, degrees);
  FrontLift.setPosition(0, degrees);

  Calibrated = true;
}

void TurnAndDrive(float x_point, float y_point, float driveSpeed, float turnSpeed, float radius, bool faceDirection, bool driveWait, float Turn_Timeout, float Drive_Timeout) {
  
  float Direction;
  
  if(faceDirection == true){
    Direction = 0;
  } else {
    Direction = 180;
  }

  TurnTo(x_point, y_point, turnSpeed, true, Direction, Turn_Timeout);
  DriveTo(x_point, y_point, driveSpeed, radius, driveWait, Drive_Timeout);
}

