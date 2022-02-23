#include "vex.h"


 vex::task Gps_X;  float GpsX;
 vex::task Gps_Y;  float GpsY;
 vex::task Gps_H;  float GpsH;
 vex::task PID;
 vex::task ControllerGPS;
 vex::task FrontLiftSensorsTask;

 bool FrontSensorsSenseATouch = false;
 bool Calibrated = false;
 int PIDsRunning = 0;
 float Distance;
 float TurnDegree;
 float TurnDistance;
 float CoustomTimeout;
 bool fb;
 bool bb;
 bool Coast;
 float MaxDistance;
 float DriveRadius;
 float DriveX;
 float DriveY;
 float TurnX;
 float TurnY;
 bool Wait;
 float Speed_V;
 float Drive_balance = -0.045;

float GetClosestToZero(float First, float Second){
  if(std::abs(First) < std::abs(Second)){
    return First;
  }else{
    return Second;
  }
}

void Calibrate(){
  
  BRMotor.spin(forward);
  BLMotor.spin(forward);
  FRMotor.spin(forward);
  FLMotor.spin(forward);
  BackLift.spin(forward);
  FrontLift.spin(forward);

  RingLiftL.setStopping(coast);
  RingLiftR.setStopping(coast);

  BRMotor.setVelocity(0,percent);
  BLMotor.setVelocity(0,percent);
  FRMotor.setVelocity(0,percent);
  FLMotor.setVelocity(0,percent);
  BackLift.setVelocity(0, percent);
  FrontLift.setVelocity(0, percent);

  BRMotor.setStopping(coast);
  BLMotor.setStopping(coast);
  FRMotor.setStopping(coast);
  FLMotor.setStopping(coast);
  BackLift.setStopping(hold);
  FrontLift.setStopping(hold);

  if(Calibrated){

    return;

  }

  BackLift.setPosition(0, degrees);
  FrontLift.setPosition(0, degrees);

  Calibrated = true;
}