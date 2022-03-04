/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       DriveTo.cpp                                               */
/*    Author:       Team 98548A                                               */
/*    Created:      8/20/2021                                                 */
/*    Description:  File that contains DriveFast() function                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

// Declare P, I, D, and RampUp values for this PID
float DKp = 100;
float DKi = 5;
float DKd = 0.1;
float DRampUpAmount = 0.125;

// base function for DriveTo()
int _DriveTo_ (){

  // Assign and declare local variables from global variables.
  float LocalCoustomTimeout = CoustomTimeout;
  float LocalDriveX = DriveX;
  float LocalDriveY = DriveY;
  float LocalMaxSpeed = Speed;
  float LocalDriveRadius = DriveRadius;
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

  // Variable that represents when the robot has first reached its destination
  float ReachedTargetTime = 0;

  // Declare variables for PID
  float Error;
  float PreviousError = 0;
  float SmartError;
  float Integral = 0;
  float Derivative;

  // This variable represents the speed that will be applied to the FrontRight motor and the BackLeft motor
  float PositiveDiagonalVoltage;

  // This variable represents the speed that will be applied to the FrontLeft motor and the BackRight motor
  float NegativeDiagonalVoltage;

  // This variable represents the voltage output of our PID controller
  float Voltage;
  
  // This variable allows the robot to smoothly accelerate to prevent slipping.
  float Ramp = 0;

  // Precalculate PreviousError
  x1 = GpsX;
  y1 = GpsY;
  PreviousError = sqrtf(powf(x1-LocalDriveX, 2) + powf(y1-LocalDriveY, 2)) - LocalDriveRadius;

  // This variable will determine wether or not the loop will continue to run
  bool NotDone = true;

  // Continue looping until done
  while (NotDone) {

    // Get the coordinate position and rotational heading of the robot
    x1 = GpsX;
    y1 = GpsY;
    h1 = GpsH;

    // calculate a point directly in front of the robot (exactly 1 inch away)
    x2 = x1 + cos(Rads(h1));
    y2 = y1 + sin(Rads(h1));

    // get Distance B and C for arccos function
    DistB = sqrt(powf(x1-LocalDriveX,2)+powf(y1-LocalDriveY,2));
    DistC = sqrt(powf(x2-LocalDriveX,2)+powf(y2-LocalDriveY,2));

    // get the positive angle from the robot to the target
    LocalTurn = Degs(acosf((powf(DistC,2)-powf(DistA,2)-powf(DistB,2))/(-2*DistA*DistB)));

    // Move the target point so that relative to the robot, the robot would be at (0, 0) and assign that point to MTX and MTY
    MTX = LocalDriveX - x1;
    MTY = LocalDriveY - y1;

    // Get rotated Y value that will determine if the robot needs to drive left or right
    RotatedY = -MTX*sin(Rads(h1))+MTY*cos(Rads(h1));

    // Make LocalTurn positive or negative depending on which direction the robot needs to drive
    LocalTurn = LocalTurn * ((RotatedY)/std::abs(RotatedY));

    // Calculate the distance from the robot to its destination
    Error = sqrtf(powf(x1-LocalDriveX, 2) + powf(y1-LocalDriveY, 2)) - LocalDriveRadius;

    // this keeps the pid moving for longer than usual when trying to grab a goal
    if(LocalExpectFrontButton || LocalExpectBackButton){
      if(Error < 5){
        Error = 5;
      }
    }
    
    // Set a 'smart error' variable to the smallest of either RampUp or Error
    SmartError = GetClosestToZero(Error, Ramp * (Error/std::abs(Error)));

    // Calculate the integral value for our PID
    Integral = Integral + Error;

    // Calculate the derivative value for our PID
    Derivative = Error - PreviousError;

    // Prevent the integral from building up until the robot is close to its destination
    if (std::abs(Error) > std::abs(LocalMaxSpeed/12)) {Integral = 0;}

    // Increase the ramp-up speed
    Ramp += DRampUpAmount;

    // Calculate the result of our PID
    Voltage = SmartError * DKp + Integral * DKi + Derivative * DKd;

    // Ensure that the robot doesn't go faster than the maximum speed
    Voltage = GetClosestToZero(Voltage, LocalMaxSpeed * (Error/std::abs(Error)));

    // Ensure that the robot doesn't go slower than the minumum voltage
    if(std::abs(Voltage)<MinVoltage){
      Voltage = MinVoltage * (std::abs(Voltage)/Voltage);
    }

    // Set PreviousError to Error
    PreviousError = Error;

    // Calculate the required speed for the wheels to allow the robot to strafe if necessary
    PositiveDiagonalVoltage = sin(Rads(LocalTurn+45))*std::abs(Voltage);
    NegativeDiagonalVoltage = -sin(Rads(LocalTurn-45))*std::abs(Voltage);

    // Apply the speeds we calculated with our PID
    FLMotor.spin(forward, NegativeDiagonalVoltage, voltageUnits::volt);
    FRMotor.spin(forward, PositiveDiagonalVoltage, voltageUnits::volt);
    BRMotor.spin(forward, NegativeDiagonalVoltage, voltageUnits::volt);
    BLMotor.spin(forward, PositiveDiagonalVoltage, voltageUnits::volt);

    // If the robot has run for longer than the coustom timeout
    if (Brain.Timer.value() > LocalCoustomTimeout) { 
      NotDone = false;
    }

    // If the robot is expecting a button to be pressed and a button is being pressed
    if (LocalExpectFrontButton && FrontSensorsSenseATouch) {
      NotDone = false;
      backAir.set(true);
    }
    if (LocalExpectBackButton && LimSwitchBack.pressing()) {
      NotDone = false;
      backAir.set(true);
    }

    // If the robot has gone past the target position and the robot is not waiting for a button to be pressed
    if (std::abs(Error)-4 <= 0.0 && !(LocalExpectBackButton || LocalExpectFrontButton)) {

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

// Wrapper function that will accept arguments for the main function (_DriveTo_())
void DriveTo(float drive_x, float drive_y, float speed, float drive_radius, bool expect_front_button, bool expect_back_button, bool wait_for_completion, float coustom_timeout){

  // Assign local variables to global variables
  DriveX = drive_x;
  DriveY = drive_y;
  Speed = (speed/100)*12;
  DriveRadius = drive_radius;
  ExpectFrontButton = expect_front_button;
  ExpectBackButton = expect_back_button;
  CoustomTimeout = coustom_timeout * 1000000;
  
  // Either wait for the function to complete, or run the function in a task
  if (wait_for_completion) {
    _DriveTo_();
  } else {
    PID = task(_DriveTo_);
  }
}