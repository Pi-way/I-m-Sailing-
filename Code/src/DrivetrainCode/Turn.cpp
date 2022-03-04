/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       Turn.cpp                                                  */
/*    Author:       Team 98548A                                               */
/*    Created:      8/20/2021                                                 */
/*    Description:  File that contains Turn() function                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

// Declare P, I, D, and RampUp values for this PID
float TKp = .5;
float TKi = .25;
float TKd = 0.01;
float TRampUpAmount = 2;

// base function for Turn()
int _Turn_() {

  // Assign and declare local variables from global variables.
  float LocalTimeout = CoustomTimeout;
  float LocalTurn = TurnDistance;
  float LocalSpeed = Speed;

  // Update PIDsRunning
  PIDsRunning ++;

  // Wait until other PIDs have completed
  while(PIDsRunning > 1){
    task::yield();
  }

  // Reset the brains timer and inertial sensor
  Inertial.setRotation(0, deg);
  Brain.Timer.reset();

  // Set the drivetrains position and brake type
  SetDriveBrake(coast);
  SetDrivePosition(0);

  // Declare variables for PID
  float Error;
  float PreviousError = 0;
  float SmartError;
  float Integral = 0;
  float Derivative;

  // Output speed of our PID speed controller
  float Speed;

  // This variable allows the robot to smoothly accelerate to prevent slipping.
  int RampUp = 0;

  // This variable wil contain the heading of the robot from the inertial sensor
  float RobotHeading;

  // This variable will determine wether or not the loop will continue to run
  bool NotDone = true;

  // Continue looping until done
  while (NotDone) {

    // Get the heading of the robot
    RobotHeading = std::abs(Inertial.rotation(degrees));

    // Calculate how much more the robot needs to turn
    Error = std::abs(LocalTurn) - RobotHeading;

    // Calculate integral
    Integral = Integral + Error;

    // Increase RampUp
    RampUp += TRampUpAmount;

    // Set a 'smart error' variable to the smallest of either RampUp or Error 
    SmartError = GetClosestToZero(RampUp, Error);

    // This is to prevent the integral from becoming too large
    if (std::abs(Error) > LocalSpeed/12 ) {
      Integral = 0;
    }

    // calculate derivative
    Derivative = Error - PreviousError;

    // Assign Error to PreviousError
    PreviousError = Error;

    // Calculate the result of our PID
    Speed = SmartError * TKp + Integral * TKi + Derivative * TKd;

    // Prevent the output speed of our PID from exceeding the maximum speed
    if (Speed > LocalSpeed) {
      Speed = LocalSpeed;
    }

    // Apply the speed from our PID to the motors. 
    if (LocalTurn > 0) {

      // Turn right if the distance we specified was positive
      FLMotor.spin(forward, Speed, voltageUnits::volt);
      FRMotor.spin(reverse, Speed, voltageUnits::volt);
      BRMotor.spin(reverse, Speed, voltageUnits::volt);
      BLMotor.spin(forward, Speed, voltageUnits::volt);
    } else {

      // Turn left if the distance we specified was positive
      FLMotor.spin(reverse, Speed, voltageUnits::volt);
      FRMotor.spin(forward, Speed, voltageUnits::volt);
      BRMotor.spin(forward, Speed, voltageUnits::volt);
      BLMotor.spin(reverse, Speed, voltageUnits::volt);
    }

    // If the robot has come within 3.5 degrees of the target or has run past the timeout value
    if (std::abs(RobotHeading) > std::abs(LocalTurn) - 3.5 || Brain.Timer.value() > LocalTimeout) {
      NotDone = false;
    }

    // Allow other tasks/threads to run
    task::yield();
  }
  
  // Stop the robot
  Drivetrain(stop(coast););

  // Update PIDsRunning
  PIDsRunning --;

  // Exit the function
  return 0;
}

// Wrapper function that will accept arguments for the main function (_Turn_())
void Turn(float Turn_, float speed, bool wait_for_completion, float CoustomTimeout_) {

  // Assign local variables to global variables
  Speed = (speed/100)*12;
  CoustomTimeout = CoustomTimeout_;
  TurnDistance = Turn_;

  // Either wait for the function to complete, or run the function in a task
  if (wait_for_completion) {
    _Turn_();
  } else {
    PID = task(_Turn_);
  }
}