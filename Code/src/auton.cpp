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
  TurnTo(-24,-24,true,0,50);
  wait(1,seconds);
  DriveTo(-24,-24, 50);
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
  FrontLiftt(100);
  Turn(85,90);
  Drive(-75,60,true,false,true,true);
  backAir.set(true);//grab first yellow goal

  TurnAndDrive(-40,2,90,90,3,false,false);
  BackLiftt(100);
  TurnAndDrive(-60,-2,90,90,16,false,false);
  task::sleep(300);
  BackLiftt(70);
  backAir.set(false);//drop yellow goal on platform

  BackLiftt(100);
  Drive(6);
  TurnAndDrive(-60,5,100,90,22);
  frontAir.set(false);//drop red goal on platform

  Drive(-6);
  TurnTo(-37,-58,90,true,0,3);
  FrontLiftt(0,false);
  DriveTo(-37,-59,100,-1,true,4);
  frontAir.set(true);//grab blue goal using front button (non-sail side)

  TurnAndDrive(-10,-10,90,90,6,false,false);
  FrontLiftt(30);
  BackLiftt(0);
  Drive(0);
  TurnAndDrive(0,0,50,80,-2,false);
  backAir.set(true);//grab middle goal

  task::sleep(300);
  BackLiftt(95, false);
  FrontLiftt(90, false);
  TurnAndDrive(60,0,100,90,18,false);
  BackLiftt(73);
  backAir.set(false);//dropping middle goal
  task::sleep(800);
  BackLiftt(75);
  Drive(5);
  TurnAndDrive(60,-3,90,80,17);
  frontAir.set(false);//dropping blue goal

  Drive(-10);
  //TurnAndDrive(30,38,100,90,0,true,false);
  task::sleep(200);
  FrontLiftt(10,false);
  TurnAndDrive(41,58,100,90,-1,true);
  frontAir.set(true);//grabbing red

  task::sleep(500);
  BackLiftt(0,false);
  FrontLiftt(30,false,70);
  //TurnAndDrive(0,38,100,100,-2,false);
  TurnTo(0,38,100,true,180);
  Drive(-50,100,false);
  wait(2,seconds);
  backAir.set(true);//grabbing yellow

  // task::sleep(200);
  // BackLiftt(90,false);
  // TurnAndDrive(-50,10,100,100,18,false);
  // backAir.set(false);
  // Turn(30);
  

  // ####################################################################
  // TurnAndDrive(52, 40, 90,80,8,false,false);
  // Drive(0);
  // BackLiftt(0);
  // task::sleep(300);
  // backAir.set(false);
  // TurnAndDrive(36,60,90,90,8,false);
  // backAir.set(true);
  // TurnAndDrive(40, 0,100,80,0,true,false);
  // BackLiftt(100);
  // FrontLiftt(90);
  // TurnTo(-60,0, 100,true,180);
  // frontAir.set(false);
  // Drive(-80);
  // backAir.set(false);



  // TurnAndDrive(-61,0,90,90,20,false);
  // BackLiftt(68);
  // task::sleep(400);
  // backAir.set(false);
  // task::sleep(500);
  // Drive(6, 100, false);
  // BackLiftt(78);
  // TurnAndDrive(60,0,90,80,16,true,false);
  // FrontLiftt(90);  
  // Drive(0);
  // frontAir.set(false);
  // TurnAndDrive(0,36,100,80,0,false);
  // Drive(-40,100);






  // TurnAndDrive(52, 40, 90,80,8,false,false);
  // Drive(0);
  // BackLiftt(0);
  // task::sleep(300);
  // backAir.set(false);
  // TurnAndDrive(36,60,90,90,8,false);
  // backAir.set(true);
  // TurnAndDrive(40, 0,100,80,0,true,false);
  // BackLiftt(100);
  // FrontLiftt(90);
  // TurnTo(-60,0, 100,true,180);
  // frontAir.set(false);
  // Drive(-80);
  // backAir.set(false);
}
void advancedSkills() {

  frontAir.set(false);
  backAir.set(false);

  DriveTo(36,60,90,8);

  frontAir.set(true);
  wait(0.25,seconds);
  FrontLiftt(100, false);

  TurnAndDrive(32,21,90,80,0,false,false); // Turn and Drive to line up with yellow nutral goal

  wait(0.7, seconds); //
  
  FrontLiftt(20, false); // put red alliance goal back down

  TurnAndDrive(0,34.5,90,80,5,false); // Turn and Drive to yellow nutral goal, grab it, and lift it up
  backAir.set(true);
  wait(0.25,seconds);
  BackLiftt(20,false);

  TurnAndDrive(-60,60,90,80,13,false,true,1);
  BackLiftt(0,false);
  backAir.set(false);
  DriveTo(-34,50,90);

  TurnAndDrive(-60,32,90,80,6,false,true,2.5); //Turn and Drive to blue goal on red platform, grab it, and lift it up
  backAir.set(true);
  wait(0.5,seconds);
  BackLiftt(100,false,30);
  FrontLiftt(100,false);

  TurnAndDrive(-38,50,90,80);


  TurnAndDrive(-38,-14.45,70,80);
  TurnAndDrive(-60,0,80,80,21);//drive to platform
  frontAir.set(false);
  Drive(-12,100);
  BackLiftt(50,false);
  FrontLiftt(0);
  TurnTo(0,0);
  Drive(100, 100, false);
  task::sleep(1000);
  frontAir.set(true);
  FrontLiftt(30);



  // frontAir.set(true);
  // FrontLiftt(100);
  // TurnAndDrive(50,0,80,80,10);
  // FrontLiftt(75);
  // frontAir.set(false);
  // FrontLiftt(80);
  // Turn(-45);


  

  


}

void easySkills() {
  Drive(21,100);                //Drive straight to line up with right red alliance goal
  Turn(87.5);                   //Turn towards the right red alliance goal
  Drive(16.5,100);              //Drive to the right red alliance goal
  frontAir.set(true);           //Grab the red alliance goal

  Drive(-35,100,false);         //Drive backwards from the goal

  FrontLiftt(25);               //Lift up the red goal so it is not on the ground

  Turn(106.5);                    //Turn to the right yellow goal
  Drive(-84,75,false);         //Drive backwards to the right yellow goal

  wait(1.75,seconds);            //Sets the clamp to lift at a midpoint in the drive
  backAir.set(true);            //Clamps the goal inpath
  BackLiftt(100,false);         //Lifts the right yellow goal up all the way during the drive to the red side

  FrontLiftt(75,false);         //Lift red alliance goal up all the way

  Turn(180);                    //Turns around to 

  FrontLiftt(10);             //Drop red goal

  Drive(11,100);
  FrontLiftt(0);
  frontAir.set(false);          //Open Front Claw
  wait(0.25,seconds);
  Drive(-5,100);

  Turn(-79, 50);

  Drive(18,100);              //Drive to platform blue goal
  frontAir.set(true);
  wait(.5, seconds);
  FrontLiftt(100, false);

  Drive(-22, 100);
  Turn(-40);
  Drive(66, 75, false);         //Drive to put the yellow goal on platform: before turn

  FrontLiftt(30, true, 50);

  Turn(-60);
  Drive(-6, 100);
  backAir.set(false);
  Drive(6,100);
  Turn(-115);                 //turn to the blue alliance goal
  Drive(-38, 100, false);
  BackLiftt(0);
  Drive(0,0);                 //helps time it.
  backAir.set(true);
  BackLiftt(25);

  Drive(12,75,false);

  BackLiftt(30, false, 50);

  Turn(95,50);

  Drive(70,100, false);
  FrontLiftt(10, true, 50);
}

/****************** Right Code  *****************/

void rightComplex() {
  backAir.set(false);
  Drive(-44, 100,false);
  wait(0.5,seconds);
  SetDriveBrake(coast);
  wait(1.75,seconds);
  backAir.set(true);
  wait(0.25,seconds);
  BackLiftt(20,false);
  Drive(28);
  Turn(-83);
  FrontLiftt(63);
  Drive(6.3);
  wait(0.5,seconds);
  frontAir.set(true);
  wait(0.5,seconds);
  Drive(-12);
  FrontLiftt(0);
  frontAir.set(false);
  Drive(18);
  frontAir.set(true);
  wait(1,seconds);
  Drive(-28,100,false);
  FrontLiftt(20);
}

void rightSimple() {
  Drive(21, 100);                //Drive straight to line up with right red alliance goal
  Turn(88, 75, false);
  FrontLiftt(65,false);
  Drive(9.2, 100);
  frontAir.set(true);
  wait(.5, sec);
  frontAir.set(false);
  Drive(-4.5, 100);
  FrontLiftt(0, true);
  Drive(8, 100, true,true,false);
  frontAir.set(true);//grab
  wait(0.25,seconds);
  Drive(-17, 100, false);
  FrontLiftt(20, true, 60);
  Turn(84.2);
  Drive(-17, 100,true,false,true,false,36);
  backAir.set(true);
  wait(0.25,seconds);
  Drive(34, 100, false);
  BackLiftt(30);
  Drive(0);
  BackLiftt(0);
  FrontLiftt(5);


}

void rightExtra() {
  Drive(42,100,true,true,false,false,60);
  frontAir.set(true);
  task::sleep(200);
  FrontLiftt(20,false);
  backAir.set(false);
  Turn(95,75,false, 1);
  wait(1, msec);
  BackLiftt(15);
  Drive(0,0); //just to make the program reset
  Drive(-20,50,false,false,true,false,35);
  wait(0.65,seconds);
  BackLiftt(0);
  Drive(0,0);
  backAir.set(true);
  task::sleep(30);
  BackLiftt(30);
  Turn(47);
  Drive(50);
  BackLiftt(0,false);
  FrontLiftt(0);

}
void none(){}
void rightWinPoint() {
  Drive(21, 100);                //Drive straight to line up with right red alliance goal
  Turn(88, 75, false);
  FrontLiftt(37,false);
  Drive(13);
  frontAir.set(true);
  Drive(-5);
  frontAir.set(false);
  FrontLiftt(-5);
  Drive(7);
  frontAir.set(true);
  wait(0.5,seconds);
  FrontLiftt(20);

  Drive(-99, 100, false);
  FrontLiftt(40);
  Turn(-109);
  BackLiftt(38,false);
  Drive(-10, 50);
  task::sleep(25000);
  backAir.set(true);
  Drive(10, 100, true);
  

}

/****************** Left Code  ******************/

void leftComplex() {
  Drive(15, 50, false);
  FrontLiftt(66, true, 90);
  task::sleep(450);
  frontAir.set(true);
  task::sleep(600);
  Drive(-6, 100, true);
  Turn(-74, 90, true);
  FrontLiftt(0, true, 90);
  frontAir.set(false);
  Drive(40, 70, true, true, false, true, 60);
  // task::sleep(2000);
  frontAir.set(true);
  task::sleep(200);
  FrontLiftt(40, true, 90);
  Drive(-42, 90, true);
  Turn(-55, 90, false);
  BackLiftt(33, false, 90);
  Drive(-6.25, 90, true);
  backAir.set(true);
  task::sleep(200);
  Drive(4, 90, true);

}

void leftSimple() {
  Drive(14.50, 50, false);
  FrontLiftt(66, true, 90);
  task::sleep(500);
  frontAir = true;
}

void leftExtra() {}

void leftWinPoint() {
  Drive(15, 50, false);
  FrontLiftt(66, true, 90);
  task::sleep(450);
  frontAir.set(true);
  task::sleep(600);
  Drive(-6, 100, true);
  Turn(-74, 100, true);
  frontAir.set(false);
  Drive(20, 100, true);
  Turn(-100, 75);
  Drive(-92.5, 75, false);
  BackLiftt(66);
  task::sleep(2100);
  backAir.set(true);
  Drive(2);
  BackLiftt(0);
  backAir.set(false);
  Drive(-10,50,true,false,true);
  backAir.set(true);
  task::sleep(100);
  Drive(20,100,false);
  BackLiftt(10);


}

void autonomous(void) {
  Brain.resetTimer();
  // Determining the auton to run based on the one selected.
  rightExtra();
  // if (alliance==SKILLS && mode == SIMPLE) {easySkills();}
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
  //     else if (mode==WINPOINT) {rightExtra();}
  //   }
  // }
}