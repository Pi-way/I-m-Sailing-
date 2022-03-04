/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       Tasks.cpp                                                 */
/*    Author:       Team 98548A                                               */
/*    Created:      8/20/2021                                                 */
/*    Description:  File that contains Tasks for GPS and our lift sensors     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

// This variable represents which GPS sensor should be used
bool GPSQuality = true; // true: Right has higher quality, false: Left has higher quality

// These preprocessor macros allow us to print to the contller screen more easily
#define C1Print(Variable) (Controller1.Screen.print(Variable))
#define C2Print(Variable) (Controller2.Screen.print(Variable))

// This function will be run in a task, continuously updating a global variable that contains the X coordinate of the robot
int GPS_X(){
  while(true){
    if(GPSQuality){
      GpsX = GPSRight.xPosition(inches);
    } else {
      GpsX = GPSLeft.xPosition(inches);
    }
    task::yield();
  }
  return 0;
}

// This function will be run in a task, continuously updating a global variable that contains the Y coordinate of the robot
int GPS_Y(){
  while(true){
    if(GPSQuality){
      GpsY = GPSRight.yPosition(inches);
    } else {
      GpsY = GPSLeft.yPosition(inches);
    }
    task::yield();
  }
  return 0;
}

// This function will be run in a task, continuously updating a global variable that contains the heading / rotation of the robot
int GPS_H(){
  while(true){
    if(GPSQuality){
      GpsH = -GPSRight.heading();
    } else {
      GpsH = -GPSLeft.heading();    
    }
    task::yield();
  }
  return 0;
}

// This function constantly prints the location of the robot to the controller as well as deciding which GPS sensor has a higher quality
int ControllerGps(){
  while(true){

    Controller1.Screen.setCursor(3,1);

    if(GPSRight.quality() > GPSLeft.quality()){
      GPSQuality = true;
      C1Print("R ");
    } else {
      GPSQuality = false;
      C1Print("L ");
    }

    C1Print("(");
    C1Print(GpsX);
    C1Print(", ");
    C1Print(GpsY);
    C1Print(", ");
    C1Print(std::round(GpsH));
    C1Print(")             ");

    task::yield();
  }
  return 0;
}

// This function constantly takes input from our distance sensors and decides if a goal is within clamping distance of the robot
int FrontLiftSensors(){

  float RightSensorDistance;
  float LeftSensorDistance;
  const int SAMPLESIZE = 1;

  while(true){

    RightSensorDistance = 0;
    LeftSensorDistance = 0;

    repeat(SAMPLESIZE){
      RightSensorDistance += DistanceRight.objectDistance(inches)/SAMPLESIZE;
      LeftSensorDistance += DistanceLeft.objectDistance(inches)/SAMPLESIZE;
      task::yield();
    } 
    
    if(RightSensorDistance < 1.6 && LeftSensorDistance < 1.6){
      FrontSensorsSenseATouch = true;
    }else{
      FrontSensorsSenseATouch = false;
    }

    task::yield();
  }
}

// This function takes all the other functions we declared above and starts them in tasks
void StartTasks(){

  Gps_X = task(GPS_X);
  Gps_Y = task(GPS_Y);
  Gps_H = task(GPS_H);
  ControllerGPS = task(ControllerGps);
  FrontLiftSensorsTask = task(FrontLiftSensors);

}