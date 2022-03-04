/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       TurnAndDrive.cpp                                          */
/*    Author:       Team 98548A                                               */
/*    Created:      8/20/2021                                                 */
/*    Description:  File that contains TurnAndDrive() function                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

void TurnAndDrive(float x_point, float y_point, float driveSpeed, float turnSpeed, float radius, bool faceDirection, bool wait_for_completion, float Turn_Timeout, float Drive_Timeout) {
  
  float Direction;
  
  if(faceDirection == true){
    Direction = 0;
  } else {
    Direction = 180;
  }

  TurnTo(x_point, y_point, turnSpeed, true, Direction, Turn_Timeout);
  DriveTo(x_point, y_point, driveSpeed, radius, wait_for_completion, Drive_Timeout);
}