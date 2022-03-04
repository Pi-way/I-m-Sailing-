#include "vex.h"

vex::task Gps_X;  float GpsX;
vex::task Gps_Y;  float GpsY;
vex::task Gps_H;  float GpsH;
vex::task PID;
vex::task ControllerGPS;
vex::task FrontLiftSensorsTask;

bool FrontSensorsSenseATouch = false;
int PIDsRunning = 0;
float Distance;
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
float Speed;
float Drive_balance = -0.045;

float GetClosestToZero(float First, float Second){
  if(std::abs(First) < std::abs(Second)){
    return First;
  }else{
    return Second;
  }
}