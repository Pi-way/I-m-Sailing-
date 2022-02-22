#include "vex.h"

float RotationG = 0;

int GPS(){
  return 0;
}

int GPS_X(){
  while(true){
    if(GPSRight.quality() > 90){
      GpsX = (GPSRight.xPosition(inches) + GPSRight.xPosition(inches) + GPSRight.xPosition(inches) + GPSRight.xPosition(inches))/4;
      task::yield();
    } else {
      GpsX = (GPSLeft.xPosition(inches) + GPSLeft.xPosition(inches) + GPSLeft.xPosition(inches) + GPSLeft.xPosition(inches))/4;
      task::yield();
    }
  }
  return 0;
}

int GPS_Y(){
  while(true){
    if(GPSRight.quality() > 90){
      GpsY = (GPSRight.yPosition(inches) + GPSRight.yPosition(inches) + GPSRight.yPosition(inches) + GPSRight.yPosition(inches))/4;
      task::yield();
    } else {
      GpsY = (GPSLeft.yPosition(inches) + GPSLeft.yPosition(inches) + GPSLeft.yPosition(inches) + GPSLeft.yPosition(inches))/4;
      task::yield();
    }
  }
  return 0;
}

int GPS_H(){
  while(true){
    if(GPSRight.quality() > 90){
      GpsH = (-GPSRight.heading() -GPSRight.heading() -GPSRight.heading() -GPSRight.heading())/4;
      RotationG = (GPSRight.rotation() + GPSRight.rotation() + GPSRight.rotation() + GPSRight.rotation())/4;
      task::yield();
    } else {
      GpsH = (-GPSLeft.heading() -GPSLeft.heading() -GPSLeft.heading() -GPSLeft.heading())/4;
      RotationG = (GPSLeft.rotation() + GPSLeft.rotation() + GPSLeft.rotation() + GPSLeft.rotation())/4;
      task::yield();      
    }
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
      task::yield();
    } else {
      Controller1.Screen.setCursor(3,1);
      Controller1.Screen.print("(");
      Controller1.Screen.print(GpsX);
      Controller1.Screen.print(", ");
      Controller1.Screen.print(GpsY);
      Controller1.Screen.print(", ");
      Controller1.Screen.print(std::round(GpsH));
      Controller1.Screen.print(")  ");
      task::yield();
    }
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

void StartGps(){
  Gps_X = task(GPS_X);
  Gps_Y = task(GPS_Y);
  Gps_H = task(GPS_H);
  ControllerGPS = task(ControllerGps);
  FrontLiftSensorsTask = task(FrontLiftSensors);
}