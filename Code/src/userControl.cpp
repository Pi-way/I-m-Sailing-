#include "vex.h"

#define Drivetrain( MotorMember )            \
  BRMotor.MotorMember;                   \
  BLMotor.MotorMember;                   \
  FRMotor.MotorMember;                   \
  FLMotor.MotorMember;


int Lift(){

  while(true){
  
    if(Controller1.ButtonL1.pressing() && BackLift.position(degrees) < 580){
      BackLift.setVelocity(100, percent);
    } else if (Controller1.ButtonL2.pressing() && BackLift.position(degrees) > 0){
      BackLift.setVelocity(-100, percent);
    } else if(Controller1.ButtonRight.pressing()) {
      BackLift.setVelocity(-100, percent);
    } else {
      BackLift.setVelocity(0,percent);
    }

    if(Controller1.ButtonR1.pressing() && FrontLift.position(degrees) < 580){
      FrontLift.setVelocity(100, percent);
    } else if (Controller1.ButtonR2.pressing() && FrontLift.position(degrees) > 20){
      FrontLift.setVelocity(-100, percent);
    } else if(Controller1.ButtonY.pressing()) {
      FrontLift.setVelocity(-100, percent);
    } else {
      FrontLift.setVelocity(0,percent);
    }

    task::sleep(20);

  }
  return 0;
}

void usercontrol(void) {

  Calibrate();
  
  task Lift_ = task(Lift);

  float Lefty;
  float Righty;
  float LeftySt;
  float RightySt;
  float AvgStrafe;
  bool Air1 = false;
  bool BP1Present = false;
  bool BP1Past = false;
  bool Air2 = false;
  bool BP2Present = false;
  bool BP2Past = false;

  while(true){

    GPS5.resetRotation();

    Righty= -Controller1.Axis3.position();
    Lefty= -Controller1.Axis2.position();

    RightySt = -Controller1.Axis4.position();
    LeftySt = -Controller1.Axis1.position();

    AvgStrafe = (LeftySt + RightySt)/2;

    FLMotor.setVelocity((Lefty + AvgStrafe),percent);
    FRMotor.setVelocity((Righty - AvgStrafe),percent);
    BLMotor.setVelocity((Lefty - AvgStrafe),percent);
    BRMotor.setVelocity((Righty + AvgStrafe),percent);

    if (Controller1.ButtonB.pressing()){
      BRMotor.setStopping(hold);
      BLMotor.setStopping(hold);
      FRMotor.setStopping(hold);
      FLMotor.setStopping(hold);
    } else {
      BRMotor.setStopping(coast);
      BLMotor.setStopping(coast);
      FRMotor.setStopping(coast);
      FLMotor.setStopping(coast);
    }
    
    //////////////////////////////////////////////////////////////

    BP1Past = BP1Present;
    BP1Present = Controller1.ButtonLeft.pressing();

    if(!BP1Past && BP1Present){
      Air1 = !Air1;
    }

    if(Air1){
      backAir.set(true);
    }else{
      backAir.set(false);
    }

    // //////////////////////////////////////////////////////////////

    BP2Past = BP2Present;
    BP2Present = Controller1.ButtonA.pressing();

    if(!BP2Past && BP2Present){
      Air2 = !Air2;
    }

    if(Air2){
      frontAir.set(true);
    }else{
      frontAir.set(false);
    }

    if(Controller1.ButtonDown.pressing() && Controller1.ButtonB.pressing()){
      Drive(-6);
      BackLiftt(66);
      Air1 = true;
      Drivetrain(spin(forward));
      BackLift.spin(forward);
    }

    //////////////////////////////////////////////////////////////

    wait(20, msec);

  }
}