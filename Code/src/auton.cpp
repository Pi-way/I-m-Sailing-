#include "vex.h"

/* AUTON SELECTOR VALUES */

#define RED 0
#define BLUE 1
#define SKILLS 2
#define LEFT 0
#define RIGHT 1
#define SIMPLE 0
#define COMPLEX 1
#define WINPOINT 2

#define SetDriveBrake(brake_mode)       \
  BRMotor.setStopping(brake_mode);      \
  BLMotor.setStopping(brake_mode);      \
  FRMotor.setStopping(brake_mode);      \
  FLMotor.setStopping(brake_mode);

void Test( void ) { // A test auton. Drives in a square 3 times, while rotating.
  frontAir.set(false);
  Drive(12,100,true,true);
  frontAir.set(true);
  Drive(-12,100);
}



/*********************\
 ***               ***
 **   A U T O N S   **
 ***               ***
\*********************/
//TurnAndDrive(float x_point, float y_point,
//             float driveSpeed = 80, float turnSpeed = 80,
//             float radius = 0, >If -1 waits for button on side opposite of sail, if -2 waits for button on sail side.
//             bool faceDirection = true,
//             bool driveWait = true,
//             float TurnTimeout = 2.5,
//             float DriveTimeout = 5);

/***************** Skills Code  *****************/
void advancedAdvancedSkills() {
  frontAir.set(false);
  frontAir.set(true);//grab red goal

  FrontLiftt(100,false);
  wait(0.75,seconds);
  Turn(85,90);
  Drive(-40,100,true,false,true,true);
  backAir.set(true);//grab first yellow goal

  TurnAndDrive(-35,5,100,100,3,false,false,2,2);
  BackLiftt(100);
  TurnAndDrive(-60,0,100,100,16,false,false,1.75,1.75);
  task::sleep(300);
  BackLiftt(75);
  wait(1,sec);
  backAir.set(false);//drop yellow goal on platform

  Drive(6);
  TurnAndDrive(-60,7,100,100,22,true,true,2.5,1.5);

  FrontLiftt(75);
  frontAir.set(false);//drop red goal on platform

  Drive(-6);
  TurnTo(-38,-58,90,true,180,3);
  BackLiftt(0,false);
  DriveTo(-38,-59,80,-2,true,2.5);
  backAir.set(true);//grab blue goal using front button (non-sail side)
  wait(.25,sec);
  tilt.set(true);

  RingLiftL.setVelocity(400,rpm);
  RingLiftR.setVelocity(400,rpm);

  RingLiftL.spin(forward);
  RingLiftR.spin(forward);

  TurnAndDrive(-10,-10,90,90,6,true,false,2.5,1.25);
  FrontLiftt(0);
  Drive(0);
  TurnAndDrive(0,0,50,80,-1,true);
  frontAir.set(true);//grab middle goal

  wait(0.25,sec);

  FrontLiftt(95, false);
  TurnAndDrive(60,0,50,100,18,true);
  FrontLiftt(65,false);
  wait(1,sec);
  frontAir.set(false);//dropping middle goal

  RingLiftL.stop();
  RingLiftR.stop();

  tilt.set(false);
  BackLiftt(80,false);

  RingLiftL.setVelocity(600,rpm);
  RingLiftR.setVelocity(600,rpm);

  RingLiftL.spin(reverse);
  RingLiftR.spin(reverse);

  Drive(-10);

  TurnAndDrive(60,12,100,100,17,false);
  backAir.set(false);//dropping blue goal

  RingLiftL.stop();
  RingLiftR.stop();

  //FrontLiftt(10,false);

  Drive(10);
  BackLiftt(10);
  TurnTo(36,60,70,true,180);
  Drive(-30,100);
  TurnTo(36,60,80,true,180);
  Drive(-10,100,false,false,true);
  BackLiftt(0,false);
  Drive(0);
  backAir.set(true);//grabbing red and tilt
  FrontLiftt(0);
  wait(0.25,seconds);
  tilt.set(true);

  RingLiftL.setVelocity(400,rpm);
  RingLiftR.setVelocity(400,rpm);

  RingLiftL.spin(forward);
  RingLiftR.spin(forward);

  TurnAndDrive(0,36,100,100,-1,true,false);
  frontAir.set(true);
  
  //TurnAndDrive(-60,-2,90,90,16,false,false);
  // Drive(0);
  // BackLiftt(70);
  // backAir.set(false);
  // BackLiftt(100);
  // Turn(45);
}
void advancedSkills() {

  backAir.set(true);
  wait(.3,sec);
  tilt.set(true);
  FrontLiftt(90);


  RingLiftL.spin(forward,400,rpm);
  RingLiftR.spin(forward,400,rpm);

  repeat(15){
    Drive(5);
    Drive(-5);
  }

  RingLiftL.setVelocity(0,percent);
  RingLiftR.setVelocity(0,percent);
}
void easySkills() {
}

/****************** Right Code  *****************/

void rightComplex() {
}
void rightSimple() {
}
void rightWinPoint() {

}

/****************** Left Code  ******************/

void leftComplex() {
}
void leftSimple() {
}
void leftWinPoint() {
}

void autonomous(void) {
  Brain.resetTimer();
  // Determining the auton to run based on the one selected.
  // advancedAdvancedSkills();
advancedAdvancedSkills();
  // if (alliance==SKILLS && mode == SIMPLE) {easySkills();}//speeeeed
  // if (alliance==SKILLS && mode == WINPOINT) {advancedAdvancedSkills();}
  // if (alliance == SKILLS && mode == COMPLEX) {advancedSkills();}
  // else {
  //   if (side==LEFT) {
  //     if (mode==SIMPLE) {leftSimple();}
  //     else if (mode==COMPLEX) {leftComplex();}
  //     else if (mode==WINPOINT) {leftWinPoint();}
  //   } else if (side==RIGHT) {
  //     if (mode==SIMPLE) {rightSimple();}
  //     else if (mode==COMPLEX) {rightComplex();}
  //     else if (mode==WINPOINT) {rightWinPoint();}
  //   }
  // }
}