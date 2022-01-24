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
  StartRingThing();
  wait(10,sec);
  StopRingThing();
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
  TurnAndDrive(-60,-4,100,100,24,false,false,1.75,1.75);
  task::sleep(300);
  BackLiftt(75);
  wait(.25,sec);
  backAir.set(false);//drop yellow goal on platform

  Drive(10);
  TurnAndDrive(-60,4,100,100,24,true,true,2.5,2);

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

  TurnAndDrive(60,8,50,100,24,true,false);
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

  //FrontLiftt(10,false);

  Drive(10);
  BackLiftt(0,false);
  FrontLiftt(2.5,false);
  // TurnTo(36,60,70,true,180);
  // Drive(-30,100);
  // TurnTo(36,60,80,true,180);
  // Drive(-10,100,false,false,true);
  // BackLiftt(0,false);
  // Drive(0);
  // backAir.set(true);//grabbing red and tilt
  // FrontLiftt(0);
  // wait(0.25,seconds);
  // tilt.set(true);

  // RingLiftL.setVelocity(400,rpm);
  // RingLiftR.setVelocity(400,rpm);

  // RingLiftL.spin(forward);
  // RingLiftR.spin(forward);

  TurnAndDrive(36,40,100,80,0,false,true,2.5,3);
  TurnAndDrive(36,60,100,80,-2,false,true);
  backAir.set(true);
  wait(.2, sec);
  tilt.set(true);
  StartRingThing();
  TurnAndDrive(0,35,100,100,-1,true,true);
  frontAir.set(true);
  TurnAndDrive(-60,6,90,90,24,true,false);
  
  FrontLiftt(75);
  Drive(0);
  frontAir.set(false);
  Drive(-5);
  Turn(-45);
  
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

  TurnAndDrive(28,-48,100,90,0,true,true); 
  FrontLiftt(10); 
  RingLiftL.spin(forward,400,rpm);
  RingLiftR.spin(forward,400,rpm);
  FrontLiftt(2.5);
  TurnAndDrive(10,-10);
  TurnAndDrive(0,0,80,90,-1,true); //Tall yellow goal
  frontAir.set(true);

  TurnAndDrive(-60,-3,90,80,17,true,false); //Drives to red platform
  FrontLiftt(85);
  RingLiftL.spin(reverse,400,rpm);
  RingLiftR.spin(reverse,400,rpm);
  tilt.set(false);
  BackLiftt(85,false);
  Drive(0);
  FrontLiftt(58);
  Turn(-20);
  wait(.8,sec);
  frontAir.set(false); //Drops tall goal on red platform
  Drive(-10);
  TurnAndDrive(-60,8,90,80,17,false,true);
  backAir.set(false); // Drops red goal on platform

  Drive(10);
  FrontLiftt(0);
  TurnAndDrive(-36,-60,90,80,-2,true,true); // drives to blue goal
  frontAir.set(true);
  BackLiftt(0);
  TurnAndDrive(0,-30,90,80,0,false,true);
  TurnAndDrive(0,-36,90,80,-1,false,true);
  backAir.set(true);
  BackLiftt(85);
  TurnAndDrive(60,-3,90,80,17,false,true);
  BackLiftt(60);
  backAir.set(false);
  Drive(12);
  FrontLiftt(85);
  TurnAndDrive(60,6,90,80,17,true,true);
  FrontLiftt(60);
  frontAir.set(false);
  Drive(-10);
  FrontLiftt(0);

  TurnAndDrive(36,60,90,80,-2,true,true); // drives to red goal
  frontAir.set(true);
  FrontLiftt(85);
  TurnAndDrive(-60,-3,90,80,17,true,false); //Drives to red platform
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


  repeat(4){
    Drive(-8);
    Drive(8);
  }
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
  Drive(2);
  repeat(7){
    Drive(8,60);
    Drive(-8,60);
  }
  //BackLiftt(15);
  tilt.set(false);

  
}
void leftWinPoint() {
  backAir.set(true);
  FrontLiftt(15);
  wait(.25, sec);
  tilt.set(true);
  Drive(6,90,false);
  StartRingThing();
  Turn(90,80);
  Drive(17);
  Turn(88,80);
  Drive(98,80,false);
  FrontLiftt(30);
  Drive(0);
  frontAir.set(true);
  Drive(-10);


}

void autonomous(void) {
  Brain.resetTimer();
  // Determining the auton to run based on the one selected.
  //advancedAdvancedSkills();

   if (alliance==SKILLS && mode == SIMPLE) {easySkills();}//speeeeed
   if (alliance==SKILLS && mode == WINPOINT) {advancedAdvancedSkills();}
   if (alliance == SKILLS && mode == COMPLEX) {advancedSkills();}
   else {
     if (side==LEFT) {
       if (mode==SIMPLE) {leftSimple();}
       else if (mode==COMPLEX) {leftComplex();}
       else if (mode==WINPOINT) {leftWinPoint();}
     } else if (side==RIGHT) {
       if (mode==SIMPLE) {rightSimple();}
       else if (mode==COMPLEX) {rightComplex();}
       else if (mode==WINPOINT) {rightWinPoint();}
     }
   }
}