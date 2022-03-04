/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       Drive.cpp                                                 */
/*    Author:       Team 98548A                                               */
/*    Created:      8/20/2021                                                 */
/*    Description:  File that contains Drive() function                       */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

// Declare P, I, D, and RampUp values for this PID
float Kp = 0.1;
float Ki = 0.1;
float Kd = 0.05;
float RampUpAmount = 4;

// base function for Drive()
int _Drive_() {

  // Assign and declare local variables from global variables.
  float LocalDistance = ((Distance/12.566))*360;
  float LocalMaxDistance = ((MaxDistance/12.566))*360;
  float LocalSpeed = Speed;
  bool LocalCoast = Coast;
  bool LocalExpectFrontButton = ExpectFrontButton;
  bool LocalExpectBackButton = ExpectBackButton;

  // Update PIDsRunning
  PIDsRunning ++;

  // Wait until other PIDs have completed
  while(PIDsRunning > 1){
    task::yield();
  }

  // Reset the brain's timer, set the drivetrain's brake type, and position
  Brain.Timer.reset();
  SetDriveBrake(hold);
  SetDrivePosition(0);

  // Declare variables for PID
  float Error;
  float PreviousError = 0;
  float SmartError;
  float Integral = 0;
  float Derivative;
  
  // This variable allows the robot to smoothly accelerate to prevent slipping.
  int RampUp = 0;

  // This variable will contain the average position of each motor in degrees
  float AverageMotorPosition;

  // Output speed of our PID speed controller
  float Speed;

  // Variable that represents when the robot has first reached its destination
  float ReachedTargetTime = 0.0;

  // Variable that represents wether or not
  bool ReachedMaximumDistance = false;

  // This variable will determine wether or not the loop will continue to run
  bool NotDone = true;

  // Continue looping until done
  while (NotDone) {

    // Calculate how far the robot has gone
    AverageMotorPosition = std::abs((FLMotor.position(degrees) + FRMotor.position(degrees) + BLMotor.position(degrees) + BRMotor.position(degrees)) / 4);

    // If the robot has gone past the maximum allowed distance
    if (AverageMotorPosition > LocalMaxDistance) {
      ReachedMaximumDistance = true;
    }

    // Calculate how much farther the robot needs to go to reach its target
    Error = std::abs(LocalDistance) - AverageMotorPosition;

    // If the robot is expecting a button to be pressed
    if(LocalExpectFrontButton || LocalExpectBackButton){
      
      // Make sure the robot doesn't go slower than the speed of our PID at 5 inches away from our target
      // This essentially is setting a lower speed limit for our PID
      if (Error < ((3/12.566))*360){
        Error = ((3/12.566))*360;
      }
    }

    // Calculate the Integral for our PID
    Integral = Integral + Error;

    // Increase the ramp-up speed
    RampUp += RampUpAmount;

    // Set a 'smart error' variable to the smallest of either RampUp or Error 
    SmartError = GetClosestToZero(Error, RampUp);
 
    // Prevent the integral from building up until the robot is close to its destination
    if (std::abs(Error) > 12) {
      Integral = 0;
    }

    // Calculate the derivative for our PID
    Derivative = Error - PreviousError;

    // Set PreviousError to Error
    PreviousError = Error;

    // Calculate the result of our PID
    Speed = SmartError * Kp + Integral * Ki + Derivative * Kd;

    // Make sure the speed result of the PID does not exeed our maximum speed
    if (Speed > LocalSpeed) {
      Speed = LocalSpeed;
    }

    // Apply the speed from our PID to the motors. 
    if (LocalDistance > 0) {

      // Apply the speed forwards if the distance we specified was positive
      FLMotor.spin(forward, Speed + Speed * Drive_balance, voltageUnits::volt);
      FRMotor.spin(forward, Speed - Speed * Drive_balance, voltageUnits::volt);
      BRMotor.spin(forward, Speed - Speed * Drive_balance, voltageUnits::volt);
      BLMotor.spin(forward, Speed + Speed * Drive_balance, voltageUnits::volt);
    } else {

      //Apply the speed backwards if the distance we specified was negative
      FLMotor.spin(reverse, Speed + Speed * Drive_balance, voltageUnits::volt);
      FRMotor.spin(reverse, Speed - Speed * Drive_balance, voltageUnits::volt);
      BRMotor.spin(reverse, Speed - Speed * Drive_balance, voltageUnits::volt);
      BLMotor.spin(reverse, Speed + Speed * Drive_balance, voltageUnits::volt);
    }

    // If the robot has gone too far
    if(ReachedMaximumDistance){ 
      NotDone = false;
    } 
    
    // If the robot has run for longer than 4 seconds
    if (Brain.Timer.value() > 4) { 
      NotDone = false;
    }

    // If the robot is expecting a button to be pressed and a button is being pressed
    if ((LocalExpectFrontButton && FrontSensorsSenseATouch)||(LocalExpectBackButton && LimSwitchBack.pressing())) { 
      NotDone = false;
    }
    
    // If the robot has gone past the target position and the robot is not waiting for a button to be pressed
    if (AverageMotorPosition > std::abs(LocalDistance) && !(LocalExpectBackButton || LocalExpectFrontButton)) {

      // Record the moment when the robot first reaches the target position
      if (ReachedTargetTime == 0){
        ReachedTargetTime = ReachedTargetTime = Brain.Timer.systemHighResolution();
      }
    }

    // If the robot has been allowed to settle in for 1/4 of a second after the moment when the robot first reached the target
    if (Brain.Timer.systemHighResolution() - ReachedTargetTime > (0.25 * 1000000) && ReachedTargetTime != 0.0){
      NotDone = false;
    }

    // Yield to allow other tasks/threads to run
    task::yield();
  }

  // Stop the robot with a specified brake type (coast or hold)
  if(LocalCoast){
    Drivetrain(stop(coast););
  }else{
    Drivetrain(stop(hold););
  }

  // Update PIDsRunning
  PIDsRunning --;

  // Exit the function
  return 0;
}

// Wrapper function that will accept arguments for the main function (_Drive_())
void Drive(float distance, float Speed_, bool wait_for_completion, bool f_b, bool b_b, bool coast, float maxDistance) {

  // Assign local variables to global variables
  Distance = distance;
  Speed = (Speed_/100)*12;
  ExpectFrontButton= f_b;
  ExpectBackButton = b_b;
  Coast = coast;
  MaxDistance = maxDistance;

  // Either wait for the function to complete, or run the function in a task
  if (wait_for_completion) {
    _Drive_();
  } else {
    PID = task(_Drive_);
  }
}