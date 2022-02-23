#include "vex.h"

int GPS(){
  return 0;
}

int GPS_X(){
  while(true){
    if(GPSRight.quality() > 90){
      GpsX = GPSRight.xPosition(inches);
    } else {
      GpsX = GPSLeft.xPosition(inches);
    }
    task::yield();
  }
  return 0;
}

int GPS_Y(){
  while(true){
    if(GPSRight.quality() > 90){
      GpsY = GPSRight.yPosition(inches);
    } else {
      GpsY = GPSLeft.yPosition(inches);
    }
    task::yield();
  }
  return 0;
}

int GPS_H(){
  while(true){
    if(GPSRight.quality() > 90){
      GpsH = -GPSRight.heading();
    } else {
      GpsH = -GPSLeft.heading();    
    }
    task::yield();
  }
  return 0;
}

int ControllerGps(){
  while(true){
    if(GPSRight.quality() > 90){
      Controller1.Screen.setCursor(3,1);
      Controller1.Screen.print("(");
      Controller1.Screen.print(GpsX);
      Controller1.Screen.print(", ");
      Controller1.Screen.print(GpsY);
      Controller1.Screen.print(", ");
      Controller1.Screen.print(std::round(GpsH));
      Controller1.Screen.print(")  ");
    } else {
      Controller1.Screen.setCursor(3,1);
      Controller1.Screen.print("(");
      Controller1.Screen.print(GpsX);
      Controller1.Screen.print(", ");
      Controller1.Screen.print(GpsY);
      Controller1.Screen.print(", ");
      Controller1.Screen.print(std::round(GpsH));
      Controller1.Screen.print(")  ");
    }
    task::yield();
  }
  return 0;
}

int FrontLiftSensors(){

  float RSD; // Right Sensor Distance
  float LSD; // Left Sensor Distance
  const int SAMPLESIZE = 1;

  while(true){

    RSD = 0;
    LSD = 0;

    repeat(SAMPLESIZE){
      RSD += DistanceRight.objectDistance(inches)/SAMPLESIZE;
      LSD += DistanceLeft.objectDistance(inches)/SAMPLESIZE;
      task::yield();
    } 
    
    if(RSD < 1.6 && LSD < 1.6){
      FrontSensorsSenseATouch = true;
    }else{
      FrontSensorsSenseATouch = false;
    }

    task::yield();
  }
}

void StartTasks(){
  Gps_X = task(GPS_X);
  Gps_Y = task(GPS_Y);
  Gps_H = task(GPS_H);
  ControllerGPS = task(ControllerGps);
  FrontLiftSensorsTask = task(FrontLiftSensors);
}