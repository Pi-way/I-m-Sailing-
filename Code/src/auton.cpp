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
  Drive(6);
  Drive(-6);
  Drive(6);
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
  Turn(93,80);
  Drive(-40,100,true,false,true,true);
  backAir.set(true);//grab first yellow goal

  TurnAndDrive(-35,5,100,100,3,false,false,2,2);
  BackLiftt(96);
  TurnAndDrive(-60,-4,100,100,20,false,false,1.75,1.75);
  task::sleep(300);
  BackLiftt(75);
  wait(.25,sec);
  backAir.set(false);//drop yellow goal on platform

  Drive(10);
  TurnAndDrive(-60,12,100,100,21,true,true,2.5,2);

  FrontLiftt(75);
  frontAir.set(false);//drop red goal on platform

  Drive(-6);
  TurnTo(-36,-36,90,true,180,3);
  TurnAndDrive(-36,-60,100,100,-2,false,false,3,3);
  BackLiftt(-2,false,70);
  Drive(0);
  backAir.set(true);//grab blue goal
  wait(.25,sec);
  tilt.set(true);

  

  TurnAndDrive(-10,-10,90,90,6,true,false,2.5,1.25);
  StartRingThing();
  FrontLiftt(2);
  Drive(0);
  TurnAndDrive(1,1,50,80,-1,true);
  frontAir.set(true);//grab middle goal
  wait(0.25,sec);

  TurnAndDrive(60,0,50,100,21,true,false); //y was 8
  FrontLiftt(85, false);  
  Drive(0);
  FrontLiftt(62,false);
  wait(1,sec);
  frontAir.set(false);//dropping middle goal

  StopRingThing();

  tilt.set(false);
  BackLiftt(80,false);

  Drive(-10);

  TurnAndDrive(60,8,100,100,26,false);
  backAir.set(false);//dropping blue goal

  RingLiftL.stop();
  RingLiftR.stop();

  Drive(10);
  BackLiftt(0,false);
  FrontLiftt(2.5,false);

  TurnAndDrive(36,40,80,80,0,false,true,2.5,3);
  TurnAndDrive(36,60,100,80,-2,false,true);
  backAir.set(true);
  wait(.25,sec);
  tilt.set(true);
  StartRingThing();

  TurnAndDrive(0,35,100,100,-1,true,true);
  frontAir.set(true);
  TurnAndDrive(-60,0,80,90,24,true,false);
  
  FrontLiftt(75);
  Drive(0);
  frontAir.set(false);
  Drive(-5);
  Turn(-90,100,false);
  wait(.5,sec);
  tilt.set(false);
  
  
  //TurnAndDrive(-60,-2,90,90,16,false,false);
  // Drive(0);w
  // BackLiftt(70);
  // backAir.set(false);
  // BackLiftt(100);
  // Turn(45);
}
void advancedSkills() {

  backAir.set(true);
  wait(.2,sec);
  tilt.set(true);
  TurnAndDrive(0,-36,100,100,-1);
  FrontLiftt(90,false);
  TurnAndDrive(-38,0,100,100,0,true);
  TurnAndDrive(-60,-5,100,100,14,true);
  FrontLiftt(72);
  frontAir.set(false);


}
void easySkills() {
}

/****************** Right Code  *****************/

void rightComplex() {
  Drive(43,100, true);
  Drive(2,70,false);
  frontAir.set(true);
  FrontLiftt(15);
  Drive(-28,100,true);
  Turn(-90);
  Drive(-16);
  backAir.set(true);
  wait(.15, sec),
  tilt.set(true);
  Drive(6);
  RingLiftL.spin(forward,400,rpm);
  RingLiftR.spin(forward,400,rpm);
  wait(.5, sec);
  Drive(24,80,false);
  FrontLiftt(0);


}
void rightSimple() {
  Drive(-12);
  backAir.set(true);
  wait(.25,sec);
  tilt.set(true);
  FrontLiftt(95);
  
  StartRingThing();

  Drive(14);
    Drive(-8);
    Drive(8);
    Drive(-8);
    Drive(8);
    Drive(-8);
    Drive(8);
    Drive(-8);
    Drive(8);
    Drive(-8);
    Drive(8);
    Drive(-8);
  Drive(12,100,false);
  wait(.25, sec);
  tilt.set(false);

}
void rightWinPoint() {
  Drive(39,100,true,true,false,true);
  frontAir.set(true);
  wait(.2, sec);
  FrontLiftt(16);
  Drive(-18);
  Turn(113.5);
  Drive(-26);
  Drive(-6,60,true,false,true,true,-12);
  backAir.set(true);
  BackLiftt(15);
  Turn(20);
  Drive(55);
  BackLiftt(0);
  FrontLiftt(0);


}

/****************** Left Code  ******************/

void leftComplex() {//untested
  backAir.set(true);
  wait(.25, sec);
  tilt.set(true);
  Turn(94,80);
  Drive(47,90,true,true);
  frontAir.set(true);
  FrontLiftt(15);
  Turn(-78,80,false);
  FrontLiftt(90);
  StartRingThing();
  Drive(18,60);
  Drive(-4);
  Turn(96);
  Drive(-42);
  FrontLiftt(0);
  tilt.set(false);

}
void leftSimple() {
  backAir.set(true);
  wait(.25, sec);
  tilt.set(true);
  FrontLiftt(50);
  StartRingThing();
    Drive(8,100);
    Drive(-8,100);
    Drive(8,100);
    Drive(-8,100);
    Drive(8,100);
    Drive(-8,100);
    Drive(8,100);
    Drive(-8,100);
    Drive(8,100);
    Drive(-8,100);
    Drive(8,100);
    Drive(-8,100);
    Drive(8,100);
    Drive(-8,100);
    Drive(8,100);
    Drive(-8,100);
  //BackLiftt(15);
  tilt.set(false);

  
}
void leftWinPoint() {
  backAir.set(true);
  FrontLiftt(15);
  wait(.25, sec);
  tilt.set(true);
  wait(.2,sec);
  StartRingThing();
  wait(1,sec);
  StopRingThing();
  Drive(6,90,false);
  tilt.set(false);
  BackLiftt(75);
  Turn(90,60);
  Drive(20,100,false);
  BackLiftt(10);
  Turn(87,60);
  Drive(70,100,false);
  FrontLiftt(0,false);
  Drive(20,100,true,true);
  frontAir.set(true);
  Turn(-40);



}
void betterLeftWinPoint() {
  backAir.set(true);
  wait(.25,sec);
  tilt.set(true);
  BackLiftt(-5,false);
  FrontLiftt(75,false);
  wait(.25,sec);
  StartRingThing();
  Drive(18,40);
  wait(1.2,sec);
  tilt.set(false);
  BackLiftt(80);

  Turn(135,70);

  Drive(31,80,false);
  wait(.25,sec);
  BackLiftt(-3,false);
  Drive(0);
  wait(.4,sec);
  tilt.set(true);
  Turn(45,60);
  Drive(64,70,false);
  FrontLiftt(58);
  wait(1,sec);
  StopRingThing();
  Drive(21,60,true,false,false,true);
  wait(.2,sec);
  frontAir.set(true);
  wait(.25,sec);
  tilt.set(false);
  Turn(-40,100,false);
  BackLiftt(0,false);



}
void midRush(){
  DriveFast(-47,100,true,false,true,false,60);
  backAir.set(true);
  wait(0.19, sec);
  tilt.set(true);
  DriveFast(45);

}
void midStandRush(){
  DriveFast(-42,100,false,false,true,false,60);
  BackLiftt(50);
  BackLiftt(0);
  Drive(0);
  backAir.set(true);
  wait(0.19, sec);
  //tilt.set(true);
  BackLiftt(10,false);
  DriveFast(40);
  BackLiftt(0);
}
void autonomous(void) {
  Brain.resetTimer();
  backAir.set(true);
  wait(0.125,sec);
  tilt.set(true);
  TurnTo(0,0);
  // Determining the auton to run based on the one selected.
//leftComplex();
//leftWinPoint();
//betterLeftWinPoint();
//advancedAdvancedSkills();
//advancedSkills();
//midRush();
//midStandRush();
//Test();
//leftSimple();

// if (alliance==SKILLS && mode == SIMPLE) {midRush();}//easySkills();}//speeeeed
//   else if (alliance==SKILLS && mode == WINPOINT) {advancedAdvancedSkills();}
//   else if (alliance == SKILLS && mode == COMPLEX) {midStandRush();}
//   else if (true){
//     if (side==LEFT) {
//       if (mode==SIMPLE) {leftSimple();}
//       else if (mode==COMPLEX) {leftComplex();}
//       else if (mode==WINPOINT) {betterLeftWinPoint();}
//     } else if (side==RIGHT) {
//       if (mode==SIMPLE) {rightSimple();}
//       else if (mode==COMPLEX) {rightComplex();}
//       else if (mode==WINPOINT) {rightWinPoint();}
//     }
//   }
}