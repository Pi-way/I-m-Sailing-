/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       TurnAndDrive.cpp                                          */
/*    Author:       Team 98548A                                               */
/*    Created:      8/20/2021                                                 */
/*    Description:  File that contains TurnAndDrive() function                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

// This function combines the functionality of TurnTo and DriveTo into one function
void TurnAndDrive(float x_point, float y_point, float drive_speed, float turn_speed, float drive_radius, bool expect_front_button, bool expect_back_button, bool face_direction, bool wait_for_completion, float turn_timeout, float drive_timeout){
  
  // This determines the side of the robot that will turn to the point
  float direction;
  if(face_direction == true){
    direction = 0;
  } else {
    direction = 180;
  }

  // Call each function respectively
  TurnTo(x_point, y_point, turn_speed, true, direction, turn_timeout);
  DriveTo(x_point, y_point, drive_speed, drive_radius, expect_front_button, expect_back_button, wait_for_completion, drive_timeout);
}