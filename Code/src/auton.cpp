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
  Drive(-40,100,true,false,true,true,-70);
  backAir.set(true);//grab first yellow goal

  TurnAndDrive(-40,2,90,90,3,false,false,2.5,2);
  BackLiftt(100);
  TurnAndDrive(-60,-2,90,90,16,false,false,2.5,1);
  task::sleep(300);
  BackLiftt(75);
  backAir.set(false);//drop yellow goal on platform

  // BackLiftt(100);
  // Drive(6);
  // TurnAndDrive(-60,5,100,90,22,true,true,2.5,1.5);
  // frontAir.set(false);//drop red goal on platform

  // Drive(-6);
  // TurnTo(-37,-58,90,true,0,3);
  // FrontLiftt(0,false);
  // DriveTo(-37,-59,100,-1,true,2.5);
  // frontAir.set(true);//grab blue goal using front button (non-sail side)

  // TurnAndDrive(-10,-10,90,90,6,false,false,2.5,1.25);
  // FrontLiftt(30);
  // BackLiftt(0);
  // Drive(0);
  // TurnAndDrive(0,0,50,80,-2,false);
  // backAir.set(true);//grab middle goal

  // task::sleep(300);
  // BackLiftt(95, false);
  // FrontLiftt(90, false);
  // TurnAndDrive(60,0,100,90,18,false);
  // BackLiftt(73);
  // backAir.set(false);//dropping middle goal
  // task::sleep(800);
  // BackLiftt(75);
  // Drive(5);
  // TurnAndDrive(60,-3,90,80,17);
  // frontAir.set(false);//dropping blue goal

  // BackLiftt(10,false);

  // TurnTo(0,38,100,true,180);
  // Drive(-30,100,false,false,true);
  // wait(0.5,seconds);
  // BackLiftt(0);
  // Drive(0);
  // backAir.set(true);//grabbing yellow
  // wait(0.5,seconds);
  // BackLiftt(100,false);

  // TurnAndDrive(-60,-2,90,90,16,false,false);
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