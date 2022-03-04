/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       Calibrate.cpp                                             */
/*    Author:       Team 98548A                                               */
/*    Created:      8/20/2021                                                 */
/*    Description:  This file contains the Calibrate function                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

// This variable ensures that the robot doesn't calibrate twice
bool Calibrated = false;

// This is the function that calibrates the robot
void Calibrate(){

  // This causes the Calibrate function to exit if it has already calibrated
  if(Calibrated){
    return;
  }
  
  // Set the position and brake type for the drivetrain motors
  SetDrivePosition(0);
  Drivetrain(setStopping(coast);)

  // Set the initial position, velocity, and brake type for both lifts
  BackLift.setPosition(0, degrees);
  FrontLift.setPosition(0, degrees);
  BackLift.setVelocity(0, percent);
  FrontLift.setVelocity(0, percent);
  BackLift.setStopping(hold);
  FrontLift.setStopping(hold);

  // Set the brake type for both ring lift motors
  RingLiftL.setStopping(coast);
  RingLiftR.setStopping(coast);

  // By telling each motor to spin forwards, we can use motor.setVelocity(). (by default motor.setVelocity() doesn't physically spin the motor, you have to call motor.spin() to cause motor.setVelocity() to work) 
  Drivetrain(spin(forward);)
  BackLift.spin(forward);
  FrontLift.spin(forward);
  RingLiftL.spin(forward);
  RingLiftR.spin(forward);

  // Set Calibrated to true to prevent multiple calibrations
  Calibrated = true;
}