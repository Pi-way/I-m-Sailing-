/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       DrivetrainStuff.cpp                                       */
/*    Author:       Team 98548A                                               */
/*    Created:      8/20/2021                                                 */
/*    Description:  Source file for DrivetrainCode and other dependendents    */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

// Declare vex::task objects for GPS, and declare corrisponding global variables.
vex::task Gps_X; float GpsX;
vex::task Gps_Y; float GpsY;
vex::task Gps_H; float GpsH;

// Declare a vex::task object that will print GPS values to the controller screen.
vex::task ControllerGPS;

// Declare a vex::task object that will run any PID loops for drivetrain.
vex::task PID;

// Declare a vex::task object to run the logic that allows the sensors on the front lift to be used.
vex::task FrontLiftSensorsTask;

// Declare a boolean variable that represents when the robot thinks a goal (or something) is within clamping reach of our pneumatic claws. This variable is continually updated via the FrontLiftSensorsTask.
bool FrontSensorsSenseATouch = false;

// Declare an integer variable that represents the number of active PIDs. This variable is utilitized by each PID controller to ensure they don't run at the same time.
int PIDsRunning = 0;

// Variables below are global variables used as a wrap-around to the fact that vex::task doesn't support passing variables into the function they run.
float Distance; 
float Speed;
float TurnDegree;
float TurnDistance;
float CoustomTimeout;
bool ExpectFrontButton;
bool ExpectBackButton;
bool Coast;
float MaxDistance;
float DriveRadius;
float DriveX;
float DriveY;
float TurnX;
float TurnY;
bool Wait;

// Declare the drive balance variable: this variable is used to compensate for any discrepancy in drivetrain friction.
float Drive_balance = -0.04;

// This function returns the number closest to zero (positive or negative)
float GetClosestToZero(float First, float Second){
  if(std::abs(First) < std::abs(Second)){
    return First;
  }else{
    return Second;
  }
}