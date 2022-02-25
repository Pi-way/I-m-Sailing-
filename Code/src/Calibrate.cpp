#include "vex.h"

void Calibrate(){
  
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
  RingLiftL.setStopping(coast);
  RingLiftR.setStopping(coast);

  BRMotor.spin(forward);
  BLMotor.spin(forward);
  FRMotor.spin(forward);
  FLMotor.spin(forward);
  BackLift.spin(forward);
  FrontLift.spin(forward);

  if(Calibrated){
    return;
  }

  BackLift.setPosition(0, degrees);
  FrontLift.setPosition(0, degrees);

  Calibrated = true;
}