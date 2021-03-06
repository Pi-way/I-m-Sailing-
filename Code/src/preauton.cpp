/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       preauton.cpp                                              */
/*    Author:       Team 98548A                                               */
/*    Created:      8/20/2021                                                 */
/*    Description:  Autonomous selector for pre-autonomous & calibration      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

#define RED 0
#define BLUE 1
#define SKILLS 2
#define LEFT 0
#define RIGHT 1
#define SIMPLE 0
#define COMPLEX 1
#define WINPOINT 2

int alliance=BLUE;
int side=LEFT;
int mode=SIMPLE;

// Auton Selector for Use Before Matches
int selectMode( void ) { 

  //Ask for mode.
  Brain.Screen.clearScreen(color::black);
  Brain.Screen.drawRectangle(50, 50, 100, 100);
  Brain.Screen.drawRectangle(200, 50, 100, 100);
  Brain.Screen.drawRectangle(350, 50, 100, 100);
  Brain.Screen.printAt(100, 50, "COMPLEX");
  Brain.Screen.printAt(250, 50, "SIMPLE");
  Brain.Screen.printAt(400, 50, "WIN POINT");
  int answer;
  int x;
  int y;
  bool waiting=true;
  while(waiting) {

    // Finds where the screen has been pressed.
    if (Controller1.ButtonY.pressing()) {
      answer = SIMPLE;
      waiting = false;
    }
    if (Brain.Screen.pressing()) {
      x = Brain.Screen.xPosition();
      y = Brain.Screen.yPosition();
      if (x>50 && x<150 && y>50 && y<150) {
        answer = COMPLEX;
        waiting = false;
      }
      else if (x>200 && x<300 && y>50 && y<150) {
        answer = SIMPLE;
        waiting = false;
      }
      else if (x>350 && x<450 && y>50 && y<150) {
        answer = WINPOINT;
        waiting = false;
      }
      while(Brain.Screen.pressing()) {
        task::sleep(50);
      }
    }
  }
  Brain.Screen.setCursor(1, 1); 

  //Returns the auton selected.
  int returnvalue;
  switch(answer) {
    case COMPLEX:
      Brain.Screen.print("You have chosen Complex.");
      returnvalue = COMPLEX;
      break;
    case SIMPLE:
      Brain.Screen.print("You have chosen Simple.");
      returnvalue = SIMPLE;
      break;
    case WINPOINT:
      Brain.Screen.print("You have chosen Win Point.");
      returnvalue = WINPOINT;
      break;  
  }
  task::sleep(1000);
  Brain.Screen.clearScreen();
  return returnvalue;
}

int selectAlliance( void ) { 

  // Selects from red, blue, and purple alliance (where purple is skills).
  Brain.Screen.drawCircle(100, 100, 50, color::red);
  Brain.Screen.drawCircle(400, 100, 50, color::blue);
  Brain.Screen.drawCircle(250, 100, 50, color::purple);
  int answer;
  int x;
  int y;
  bool waiting = true;
  while(waiting) {
    if (Controller1.ButtonY.pressing()) {
      answer = RED;
      waiting = false;
    }

    //Gets the position of the press on the screen.
    if (Brain.Screen.pressing()) {
      x = Brain.Screen.xPosition();
      y = Brain.Screen.yPosition();
      if (x>50 && x<150 && y>50 && y<150) {
        answer = RED;
        waiting = false;
      }
      else if (x>350 && x<450 && y>50 && y<150) {
        answer = BLUE;
        waiting = false;
      }
      else if (x>200 && x<300 && y>50 && y<150) {
        answer = SKILLS;
        waiting = false;
      }
      while (Brain.Screen.pressing()) {
        task::sleep(50);
      }
    }
  }
  int returnvalue; 

  //Returns the alliance selected.
  switch (answer) {
  case 1:
    Brain.Screen.clearScreen(color::blue);
    task::sleep(500);
    Brain.Screen.clearScreen(color::black);
    returnvalue = BLUE;
    break;
  case 0:
    Brain.Screen.clearScreen(color::red);
    task::sleep(500);
    Brain.Screen.clearScreen(color::black);
    returnvalue = RED;
    break;
  case 2:
    Brain.Screen.clearScreen(color::purple);
    task::sleep(500);
    Brain.Screen.clearScreen(color::black);
    returnvalue = SKILLS;
    break;
  }
  return returnvalue;
}

int selectSide( void ) {

  // Selects left or right starting position.
  Brain.Screen.clearScreen(color::black);
  Brain.Screen.drawRectangle(50, 50, 100, 100);
  Brain.Screen.drawRectangle(200, 50, 100, 100);
  Brain.Screen.printAt(100, 50, "LEFT");
  Brain.Screen.printAt(250, 50, "RIGHT");
  int answer;
  int x;
  int y;
  bool waiting=true;
  while(waiting) {
    if (Controller1.ButtonY.pressing()) {
      answer = LEFT;
      waiting = false;
    }

    // Gets the position of the press on the screen.
    if (Brain.Screen.pressing()) {
      x = Brain.Screen.xPosition();
      y = Brain.Screen.yPosition();
      if (x>50 && x<150 && y>50 && y<150) {
        answer = LEFT;
        waiting = false;
      }
      else if (x>200 && x<300 && y>50 && y<150) {
        answer = RIGHT;
        waiting = false;
      }
      while(Brain.Screen.pressing()) {
        task::sleep(50);
      }
    }
  }
  Brain.Screen.setCursor(1, 1); 

  //Returns the side selected.
  bool returnvalue;
  switch(answer) {
    case LEFT:
      Brain.Screen.print("You have chosen Left.");
      task::sleep(1000);
      Brain.Screen.clearScreen();
      returnvalue = LEFT;
      break;
    case RIGHT:
      Brain.Screen.print("You have chosen Right.");
      task::sleep(1000);
      Brain.Screen.clearScreen();
      returnvalue = RIGHT;
      break;
  }
  return returnvalue;
}

void  pre_auton( void ) {

  Calibrate();
  Inertial.calibrate();
  GPSLeft.calibrate();
  GPSRight.calibrate();
  StartGps();
  wait(3,seconds);
  vexcodeInit();
  alliance = selectAlliance();
  side = selectSide();
  mode = selectMode();

  //Prints the modes chosen.
  if (alliance==RED) {Brain.Screen.printAt(1, 20, "Alliance: Red");}
  else if (alliance==BLUE) {Brain.Screen.printAt(1,20,"Alliance: Blue");}
  else {Brain.Screen.printAt(1,20,"Alliance: Skills");}

  if (side==LEFT) {Brain.Screen.printAt(1,40,"Side: Left");}
  else {Brain.Screen.printAt(1,40,"Side: Right");}

  if (mode==SIMPLE) {Brain.Screen.printAt(1,60,"Mode: Simple");}
  else if (mode==COMPLEX) {Brain.Screen.printAt(1,60,"Mode: Complex");}
  else {Brain.Screen.printAt(1, 60, "Mode: Win Point");}

}