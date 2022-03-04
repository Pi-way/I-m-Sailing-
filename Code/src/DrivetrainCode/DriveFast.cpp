/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       DriveFast.cpp                                             */
/*    Author:       Team 98548A                                               */
/*    Created:      8/20/2021                                                 */
/*    Description:  File that contains DriveFast() function                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

// Declare P, I, and D values for this PID
float fKp = 0.05;
float fKi = 0.01;
float fKd = 0.1;
float fRampUp = 60;

// base function for DriveFast()
int _DriveFast_() {

  // Assign and declare local variables from global variables.
  float LocalDistance = ((Distance/12.566))*360;
  float LocalSpeed = Speed;
  bool LocalCoast = Coast;
  float LocalMaxDistance = ((MaxDistance/12.566))*360;
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
  float smartError;
  float Integral = 0;
  float Derivative;
  
  // This variable allows the robot to smoothly accelerate to prevent slipping.
  int RampUp = 0;

  // This variable will contain the average position of each motor in degrees
  float AverageMotorPosition;

  // Output speed of our PID speed controller
  float Speed;

  // Variable that represents when
  float StartEndTime = 0.0;

  bool STAHP = false;

  bool Condition = true;
  while (Condition) {

    AverageMotorPosition = std::abs((FLMotor.position(degrees) + FRMotor.position(degrees) + BLMotor.position(degrees) + BRMotor.position(degrees)) / 4);

    Error = std::abs(LocalDistance) - AverageMotorPosition;

    if(Error < ((5/12.566))*360 && (LocalExpectFrontButton || LocalExpectBackButton)){
      if (AverageMotorPosition > LocalMaxDistance){
        STAHP = true;
      }else{
        Error = ((5/12.566))*360;
      }
    }

    Integral = Integral + Error;

    smartError = GetClosestToZero(Error, RampUp);
 

    if (Error == 0) {
      Integral = 0;
    }

    if (std::abs(Error) > LocalSpeed ) {
      Integral = 0;
    }

    Derivative = Error - PreviousError;
    PreviousError = Error;

    Speed = smartError * fKp + Integral * fKi + Derivative * fKd;

    RampUp += fRampUp;

    if (Speed > LocalSpeed) {
      Speed = LocalSpeed;
    }

    if (LocalDistance > 0) {
    FLMotor.spin(forward, Speed + Speed * Drive_balance, voltageUnits::volt);
    FRMotor.spin(forward, Speed - Speed * Drive_balance, voltageUnits::volt);
    BRMotor.spin(forward, Speed - Speed * Drive_balance, voltageUnits::volt);
    BLMotor.spin(forward, Speed + Speed * Drive_balance, voltageUnits::volt);
    } else {
      FLMotor.spin(reverse, Speed + Speed * Drive_balance, voltageUnits::volt);
      FRMotor.spin(reverse, Speed - Speed * Drive_balance, voltageUnits::volt);
      BRMotor.spin(reverse, Speed - Speed * Drive_balance, voltageUnits::volt);
      BLMotor.spin(reverse, Speed + Speed * Drive_balance, voltageUnits::volt);
    }

    if ((AverageMotorPosition > std::abs(LocalDistance) && !(LocalExpectFrontButton || LocalExpectBackButton)) || Brain.Timer.value() > 3 || ((LocalExpectFrontButton && FrontSensorsSenseATouch)||(LocalExpectBackButton && LimSwitchBack.pressing())) || STAHP) {    //Was: avgm > std::abs(LocalDistance) - 40 || Brain.Timer.value() > 4

      if (StartEndTime == 0.0){
        StartEndTime = Brain.Timer.systemHighResolution();
      }

      if (LocalExpectFrontButton || LocalExpectBackButton || STAHP) {
        if(Brain.Timer.systemHighResolution() - StartEndTime > 250000 || Brain.Timer.value() > 3){
          Condition = false;
        }
        if (STAHP){
          Condition = false;
        }
      } else {
        Condition = false;
      }
    }

    task::yield();

  }

  if(LocalCoast){
    Drivetrain(stop(hold););
  }else{
    Drivetrain(stop(hold););
  }

  PIDsRunning --;

  return 0;

}

// Wrapper function that will accept arguments for the main function (_DriveFast_())
void DriveFast(float distance, float speed, bool wait_for_completion, bool expect_front_button, bool expect_back_button, bool coast, float max_distance) {

  // Assign local variables to global variables
  Distance = distance;
  Speed = (speed/100)*12; // convert percent to voltage
  ExpectFrontButton = expect_front_button;
  ExpectBackButton = expect_back_button;
  Coast = coast;
  MaxDistance = max_distance;

  // Either wait for the function to complete, or run the function in a task
  if (wait_for_completion) {
    _DriveFast_();
  } else {
    PID = task(_DriveFast_);
  }

}