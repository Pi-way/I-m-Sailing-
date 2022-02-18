/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       preauton.cpp                                              */
/*    Author:       Team 98548A                                               */
/*    Created:      8/20/2021                                                 */
/*    Description:  Contains code for user controll                           */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

// this is the function that controlls the lifts and ring intake:
int Lift(){
	
  bool Roller = false;
  bool BP3Present = false;
  bool BP3Past = false;

  while(true){
  
    // IF we are telling the lift to go down and it can still go down -OR- we are telling the lift to go down no matter what...
    if((Controller1.ButtonL1.pressing() && Controller1.ButtonL2.pressing()) || (Controller1.ButtonL2.pressing() && FrontLift.position(degrees) > 0)){
      FrontLift.setVelocity(-100, percent);
    } else if(Controller1.ButtonL1.pressing() && FrontLift.position(degrees) < 580) {
      FrontLift.setVelocity(100, percent);
    } else {
      FrontLift.setVelocity(0,percent);
    }

    if((Controller1.ButtonR1.pressing() && Controller1.ButtonR2.pressing()) || (Controller1.ButtonR2.pressing() && BackLift.position(degrees) > 18)) {
      BackLift.setVelocity(-100, percent);
    } else if(Controller1.ButtonR1.pressing() && BackLift.position(degrees) < 580){
      BackLift.setVelocity(100, percent);
    } else {
      BackLift.setVelocity(0,percent);
    }
    
    BP3Past = BP3Present;
    BP3Present = Controller2.ButtonR1.pressing();

    if(!BP3Past && BP3Present){
      Roller = !Roller;
    }
    
    if(Controller2.ButtonR2.pressing()) {
      RingLiftL.spin(reverse,400,rpm);
      RingLiftR.spin(reverse,400,rpm);
    } else if (Roller){
      RingLiftL.spin(forward,400,rpm);
      RingLiftR.spin(forward,400,rpm);
    } else {
      RingLiftL.stop(coast);
      RingLiftR.stop(coast);
    }

    task::yield();

  }
  return 0;
}

void usercontrol(void) {

  Calibrate();
  
  task Lift_ = task(Lift);

  float Lefty;
  float Righty;
  // float LeftySt;
  // float RightySt;
  // float AvgStrafe;
  bool Air1 = false;
  bool BP1Present = false;
  bool BP1Past = false;
  bool Air2 = false;
  bool BP2Present = false;
  bool BP2Past = false;
  bool Air4 = false;
  bool BP4Present = false;
  bool BP4Past = false;

  while(true){

    Lefty= Controller1.Axis3.position();
    Righty= Controller1.Axis2.position();

    // RightySt = -Controller1.Axis4.position();
    // LeftySt = -Controller1.Axis1.position();

    // AvgStrafe = (LeftySt + RightySt)/2;

    // FLMotor.setVelocity((Lefty - AvgStrafe),percent);
    // FRMotor.setVelocity((Righty + AvgStrafe),percent);
    // BLMotor.setVelocity((Lefty + AvgStrafe),percent);
    // BRMotor.setVelocity((Righty - AvgStrafe),percent);
    
    FLMotor.setVelocity(Lefty,pct);
    FRMotor.setVelocity(Righty,pct);
    BLMotor.setVelocity(Lefty,pct);
    BRMotor.setVelocity(Righty,pct);

    if (Controller2.ButtonB.pressing()){
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

    BP1Past = BP1Present;
    BP1Present = Controller1.ButtonLeft.pressing();

    if(!BP1Past && BP1Present){
      Air1 = !Air1;
    }

    if(Air1){
      frontAir.set(true);
    }else{
      frontAir.set(false);
    }

    BP2Past = BP2Present;
    BP2Present = Controller1.ButtonA.pressing();

    if(!BP2Past && BP2Present){
      Air2 = !Air2;
    }

    if(Air2){
      backAir.set(true);
    }else{
      backAir.set(false);
    }

    BP4Past = BP4Present;
    BP4Present = Controller2.ButtonL1.pressing();

    if(!BP4Past && BP4Present){
      Air4 = !Air4;
    }

    if(Air4){
      tilt.set(true);
    }else{
      tilt.set(false);
    }

    task::yield();
    
  }
}