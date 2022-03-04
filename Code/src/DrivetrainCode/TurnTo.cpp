/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       TurnTo.cpp                                                */
/*    Author:       Team 98548A                                               */
/*    Created:      8/20/2021                                                 */
/*    Description:  File that contains TurnTo() function                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

// Declare P, I, D, and RampUp values for this PID
float TTKp = .5;
float TTKi = .25;
float TTKd = 0.01;
float TTRampUpAmount = 1;

// base function for TurnTo()
int _TurnTo_() {

  // Assign and declare local variables from global variables.
  float LocalTimeout = CoustomTimeout;
  float LocalTurnX = TurnX;
  float LocalTurnY = TurnY;
  float LocalMaxSpeed = Speed;
  float LocalTurnDegree = TurnDegree;

  // Update PIDsRunning
  PIDsRunning ++;

  // Wait until other PIDs have completed
  while(PIDsRunning > 1){
    task::yield();
  }

  // These variables represent the X and Y coordinates, and the rotation of the robot
  float x1;
  float y1;
  float h1;

  // These variables represent a point directly in front of the robot
  float x2;
  float y2;

  // These variables represent the distances between each point we use
  float DistA = 1.0;
  float DistB;
  float DistC;

  // These variables represent the target point after moving the robot to the origin (mathematically)
  float MTX;
  float MTY;

  // RotatedY represents the Y-coordinate of the target point after moving it to the origin, and rotating it to adjust for the heading of the robot
  float RotatedY;

  // Local Turn represents how many more degrees the robot needs to turn to face the target point
  float LocalTurn;

  // Set the drivetrain's brake mode and position
  SetDriveBrake(brake);
  SetDrivePosition(0);

  // Variable that represents when the robot has first reached its destination
  float ReachedTargetTime = 0;

  // Declare variables for PID
  float Error;
  float PreviousError = 0;
  float SmartError;
  float Integral = 0;
  float Derivative;

  // Output speed of our PID speed controller
  float Voltage;

  // This variable allows the robot to smoothly accelerate to prevent slipping.
  int RampUp = 0;

  // This variable will determine wether or not the loop will continue to run
  bool NotDone = true;

  // Continue looping until done
  while (NotDone) {

    // Get the coordinate position and rotational heading of the robot
    x1 = GpsX;
    y1 = GpsY;
    h1 = GpsH + LocalTurnDegree;

    // calculate a point directly in front of the robot (exactly 1 inch away)
    x2 = x1 + cos(Rads(h1));
    y2 = y1 + sin(Rads(h1));

    // get Distance B and C for arccos function
    DistB = sqrt(powf(x1-LocalTurnX,2)+powf(y1-LocalTurnY,2));
    DistC = sqrt(powf(x2-LocalTurnX,2)+powf(y2-LocalTurnY,2));

    // get the positive angle from the robot to the target
    LocalTurn = Degs(acosf((powf(DistC,2)-powf(DistA,2)-powf(DistB,2))/(-2*DistA*DistB)));

    // Move the target point so that relative to the robot, the robot would be at (0, 0) and assign that point to MTX and MTY
    MTX = LocalTurnX - x1;
    MTY = LocalTurnY - y1;

    // Get rotated Y value that will determine if the robot needs to drive left or right
    RotatedY = -MTX*sin(Rads(h1))+MTY*cos(Rads(h1));

    // Make LocalTurn positive or negative depending on which direction the robot needs to drive
    LocalTurn = LocalTurn * ((RotatedY)/std::abs(RotatedY));

    // Set error to the amount the robot needs to turn to reach the target
    Error = LocalTurn;
   
    // Calculate the Integral for our PID
    Integral = Integral + Error;

    // Prevent the integral from becoming too large
    if(Integral > 40)(Integral = 40);
    if (std::abs(Voltage) > 5) {Integral = 0;}

    // Calculate the derivative for our PID
    Derivative = Error - PreviousError;
    
    // Increase RampUp
    RampUp += TTRampUpAmount;

    // Assign Error to PreviousError
    PreviousError = Error;

    // Set a 'smart error' variable to the smallest of either RampUp or Error 
    SmartError = GetClosestToZero(Error, RampUp * (Error/std::abs(Error)));

    // Calculate the result of our PID
    Voltage = SmartError * TTKp + Integral * TTKi + Derivative * TTKd;

    // Ensure that the result of our PID doesn't exceed the maximum speed
    Voltage = GetClosestToZero(Voltage, LocalMaxSpeed * (Error/std::abs(Error)));

    // Ensure that the result of our PID doesn't go below the minnimum speed
    if(std::abs(Voltage)<MinVoltage){
      Voltage = MinVoltage * (std::abs(Voltage)/Voltage);
    }

    // Apply the result of our PID to the motors
    FLMotor.spin(forward, Voltage, voltageUnits::volt);
    FRMotor.spin(reverse, Voltage, voltageUnits::volt);
    BRMotor.spin(reverse, Voltage, voltageUnits::volt);
    BLMotor.spin(forward, Voltage, voltageUnits::volt);

    // If the robot has run for longer than the LocalTimeout
    if (Brain.Timer.value() > LocalTimeout) { 
      NotDone = false;
    }
    
    // If the robot has gone past the target position
    if (std::abs(LocalTurn)-.5 <= 0) {

      // Record the moment when the robot first reaches the target position
      if (ReachedTargetTime == 0){
        ReachedTargetTime = ReachedTargetTime = Brain.Timer.systemHighResolution();
      }
    }

    // If the robot has been allowed to settle in for 1/4 of a second after the moment when the robot first reached the target
    if (Brain.Timer.systemHighResolution() - ReachedTargetTime > (0.25 * 1000000) && ReachedTargetTime != 0.0){
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

// Wrapper function that will accept arguments for the main function (_TurnTo_())
void TurnTo(float x_cooridnate, float y_coordinate, float speed, bool wait_for_completion, float turn_offset, float coustom_timeout) {

  // Assign local variables to global variables
  Speed = (speed/100)*12;
  TurnDegree = turn_offset;
  CoustomTimeout = coustom_timeout * 1000000;
  TurnX = x_cooridnate;
  TurnY = y_coordinate;

  // Either wait for the function to complete, or run the function in a task
  if (wait_for_completion) {
    _TurnTo_();
  } else {
    PID = task(_TurnTo_);
  }
}