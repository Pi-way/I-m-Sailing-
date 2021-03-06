#include "vex.h"

#define SetDriveBrake(brake_mode)       \
  BRMotor.setStopping(brake_mode);      \
  BLMotor.setStopping(brake_mode);      \
  FRMotor.setStopping(brake_mode);      \
  FLMotor.setStopping(brake_mode);

#define SetDrivePosition(DrivePosition)               \
  BRMotor.setPosition((DrivePosition), degrees);      \
  BLMotor.setPosition((DrivePosition), degrees);      \
  FRMotor.setPosition((DrivePosition), degrees);      \
  FLMotor.setPosition((DrivePosition), degrees);

#define Drivetrain( MotorMember )            \
  BRMotor.MotorMember                      \
  BLMotor.MotorMember                      \
  FRMotor.MotorMember                      \
  FLMotor.MotorMember

#define Rads(Degs) ((3.1415926535/180.0)*Degs)
#define Degs(Rads) ((180.0/3.1415926535)*Rads)

#define MinVoltage 1.5

float GetClosestToZero(float First, float Second){
  if(std::abs(First) < std::abs(Second)){
    return First;
  }else{
    return Second;
  }
}

bool FrontSensorsSenseATouch = false;

bool Calibrated = false;
int PIDsRunning = 0;
float Drive_balance = 0;//-0.045
float TKp = 0.1;
float TKi = 0.025;//1;
float TKd = 0;//1;

float Kp = 0.05;
float Ki = 0.1;
float Kd = 0.1;


float fKp = 0.04;
float fKi = 0.01;
float fKd = 0.1;

float DKp = 0.02;
float DKi = 0.0;//000005;
float DKd = 0.1;

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
bool DriveForwards;

task PID;
task TURN_PID;

//GPS stuff:
task Gps_X; float GpsX;
task Gps_Y; float GpsY;
task Gps_H; float GpsH;
float RotationG = 0;
task ControllerGPS;
task FrontLiftSensorsTask;

float Speed_V;

int GPS(){
  return 0;
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

int _Drive_Fast() {
  float SessionDistance = ((Distance/12.566)/1.4)*360;
  bool SessionWait = Wait;
  float SessionSpeed_V = Speed_V;
  bool SessionCoast = Coast;
  float SessionMaxDistance = MaxDistance;

  bool FrontButton = fb;
  bool BackButton = bb;

  PIDsRunning ++;

  while(PIDsRunning > 1){
    task::yield();
  }

  Brain.Timer.reset();

  SetDriveBrake(hold);

  FLMotor.setPosition(0, degrees);
  FRMotor.setPosition(0, degrees);
  BLMotor.setPosition(0, degrees);
  BRMotor.setPosition(0, degrees);

  bool Condition = true;

  float Error;
  float Integral = 0;
  float PreviousError = 0;
  float Derivative;
  float Speed;
  float smartError;
  int x = 0;
  float avgm;

  float StartEndTime = 0.0;

  bool STAHP = false;

  while (Condition) {

    avgm = std::abs((FLMotor.position(degrees) + FRMotor.position(degrees) + BLMotor.position(degrees) + BRMotor.position(degrees)) / 4);

    Error = std::abs(SessionDistance) - avgm;

    if(Error < ((5/12.566)/1.4)*360 && (FrontButton || BackButton)){
      if (avgm > ((SessionMaxDistance/12.566)/1.4)*360){
        STAHP = true;
      }else{
        Error = ((5/12.566)/1.4)*360;
      }
    }

    Integral = Integral + Error;

    if (Error <= x) {
      smartError = Error;
    } else if (x <= Error) {
       smartError = x;
    }
 

    if (Error == 0) {
      Integral = 0;
    }

    if (std::abs(Error) > SessionSpeed_V ) {
      Integral = 0;
    }

    Derivative = Error - PreviousError;
    PreviousError = Error;

    Speed = smartError * fKp + Integral * fKi + Derivative * fKd;

    x += 60;

    if (Speed > SessionSpeed_V) {
      Speed = SessionSpeed_V;
    }

    if (SessionDistance > 0) {
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
    if(FrontButton && FrontSensorsSenseATouch){
      frontAir.set(true);
    } else if(BackButton && LimSwitchBack.pressing()){
      backAir.set(true);
    }
    if ((avgm > std::abs(SessionDistance) && !(FrontButton || BackButton)) || Brain.Timer.value() > 3 || ((FrontButton && FrontSensorsSenseATouch)||(BackButton && LimSwitchBack.pressing())) || STAHP) {    //Was: avgm > std::abs(SessionDistance) - 40 || Brain.Timer.value() > 4

      if (StartEndTime == 0.0){
        StartEndTime = Brain.Timer.systemHighResolution();
      }

      if (FrontButton || BackButton || STAHP) {
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

    if (SessionWait) {
      wait(20, msec);
    } else {
      task::yield();
    }

  }

  if(SessionCoast){
    Drivetrain(stop(hold););
  }else{
    Drivetrain(stop(hold););
  }

  PIDsRunning --;

  return 0;

}

void DriveFast(float Distance_, float Speed_V_, bool Wait_, bool f_b, bool b_b, bool coast, float maxDistance) {

  Distance = Distance_;
  Wait = Wait_;
  Speed_V = (Speed_V_/100)*12;
  fb= f_b;
  bb = b_b;
  Coast = coast;
  MaxDistance = maxDistance;

  wait(20, msec);

  if (Wait) {

    _Drive_Fast();

  } else {

    PID = task(_Drive_Fast);

  }

  wait(20, msec);

}

int _Drive_() {
  float SessionDistance = ((Distance/12.566)/1.4)*360;
  bool SessionWait = Wait;
  float SessionSpeed_V = Speed_V;
  bool SessionCoast = Coast;
  float SessionMaxDistance = MaxDistance;

  bool FrontButton = fb;
  bool BackButton = bb;

  PIDsRunning ++;

  while(PIDsRunning > 1){
    task::yield();
  }

  Brain.Timer.reset();

  SetDriveBrake(hold);

  FLMotor.setPosition(0, degrees);
  FRMotor.setPosition(0, degrees);
  BLMotor.setPosition(0, degrees);
  BRMotor.setPosition(0, degrees);

  bool Condition = true;

  float Error;
  float Integral = 0;
  float PreviousError = 0;
  float Derivative;
  float Speed;
  float smartError;
  int Ramp = 0;
  float avgm;

  float StartEndTime = 0.0;

  bool STAHP = false;

  while (Condition) {

    avgm = std::abs((FLMotor.position(degrees) + FRMotor.position(degrees) + BLMotor.position(degrees) + BRMotor.position(degrees)) / 4);
    Error = std::abs(SessionDistance) - avgm;

    if(Error < ((3/12.566)/1.4)*360 && (FrontButton || BackButton)){ //If error is less than ~3 inch AND you are waiting for a goal to touch
      if (std::abs(avgm) > std::abs(((SessionMaxDistance/12.566)/1.4)*360)){ //If you have gone too far
        STAHP = true;
      }else{ //you haven't reached the goal and haven't gone too far.
        Error = ((3/12.566)/1.4)*360; 
      }
    }

    Integral = Integral + Error;
    Derivative = Error - PreviousError;
    PreviousError = Error;
    Ramp += 4;

    if (Error <= Ramp) { smartError = Error; }
    else if (Ramp <= Error) { smartError = Ramp; }
    if (Error == 0) { Integral = 0; }
    if (std::abs(Error) > SessionSpeed_V ) { Integral = 0; }

    Speed = smartError * Kp + Integral * Ki + Derivative * Kd;

    if (Speed > SessionSpeed_V) { Speed = SessionSpeed_V; }

    if (SessionDistance > 0) {
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

    if ((avgm > std::abs(SessionDistance) && !(FrontButton || BackButton)) || Brain.Timer.value() > 3 || ((FrontButton && FrontSensorsSenseATouch)||(BackButton && LimSwitchBack.pressing())) || STAHP) {    //Was: avgm > std::abs(SessionDistance) - 40 || Brain.Timer.value() > 4

      if (StartEndTime == 0.0){
        StartEndTime = Brain.Timer.systemHighResolution();
      }

      if (FrontButton || BackButton || STAHP) {
        if(Brain.Timer.systemHighResolution() - StartEndTime >= 0 || Brain.Timer.value() > 4){
          Condition = false;
        }
        if (STAHP){
          Condition = false;
        }
      } else {
        Condition = false;
      }
    }

    if (SessionWait) {
      wait(20, msec);
    } else {
      task::yield();
    }

  }

  if(SessionCoast){
    Drivetrain(stop(coast););
  }else{
    Drivetrain(stop(););
  }

  PIDsRunning --;

  return 0;

}

void Drive(float Distance_, float Speed_V_, bool Wait_, bool f_b, bool b_b, bool coast, float maxDistance) {

  Distance = Distance_;
  Wait = Wait_;
  Speed_V = (Speed_V_/100)*12;
  fb= f_b;
  bb = b_b;
  Coast = coast;
  MaxDistance = maxDistance;

  wait(20, msec);

  if (Wait) {

    _Drive_();

  } else {

    PID = task(_Drive_);

  }

  wait(20, msec);

}

int _DriveTo_ (){

  //Pull global variables into Drive Function:
  float SessionTimeout = CoustomTimeout;
  float SessionDriveX = DriveX;
  float SessionDriveY = DriveY;
  float SessionMaxSpeed = Speed_V;
  float SessionDriveRadius;
  bool LimitSwitchFront;
  bool LimitSwitchBack;
  int DirectionalFactor = 0;

  if(DriveForwards){
    DirectionalFactor = 1;
  }else{
    DirectionalFactor = -1;
  }

  if (DriveRadius == -1.0){
    SessionDriveRadius = 0;
    LimitSwitchFront = true;
  }else{
    SessionDriveRadius = DriveRadius;
    LimitSwitchFront = false;
  }

  if (DriveRadius == -2.0){
    SessionDriveRadius = 0;
    LimitSwitchBack = true;
  }else{
    SessionDriveRadius = DriveRadius;
    LimitSwitchBack = false;
  }

  //Wait untill other PIDs are done to prevent conflicting motor commands:
  PIDsRunning ++;
  WaitUntil(PIDsRunning == 1);

  float StartTime = Brain.Timer.systemHighResolution();
  SetDriveBrake(hold);
  SetDrivePosition(0);

  float x1; float y1;
  float h1;                 //                         Variables for 
  float x2; float y2;       //                      law-of-cosines math
  float DistA = 1.0; float DistB;float DistC;
  float MTX; float MTY; 
  float RotatedY; //      Variables for rotated plane
  float SessionTurn;

  //float PositiveDiagonalVoltage;
  //float NegativeDiagonalVoltage;

  bool Condition = true;
  bool ReachedTarget = false;
  float ReachedTargetTime = 0;

  float Integral;
  float Derivative;
  float PreviousError;

  float Error;
  float OverallSpeed;
  //float SideDifferenceSpeed;
  float SmartVoltage;
  float Ramp = 0;

  x1 = GpsX;
  y1 = GpsY;

  PreviousError = sqrtf(powf(x1-SessionDriveX, 2) + powf(y1-SessionDriveY, 2)) - SessionDriveRadius;

  float RightSpeed = 0;
  //float LeftSpeed = 0;

  while (Condition) {

    x1 = GpsX;
    y1 = GpsY;
    h1 = GpsH;
    x2 = x1 + cos(Rads(h1))*DirectionalFactor;
    y2 = y1 + sin(Rads(h1))*DirectionalFactor;
    DistB = sqrt(powf(x1-SessionDriveX,2)+powf(y1-SessionDriveY,2));                          //get Distance B for arccos function
    DistC = sqrt(powf(x2-SessionDriveX,2)+powf(y2-SessionDriveY,2));                          //get Distance C for arccos function
    SessionTurn = Degs(acosf((powf(DistC,2)-powf(DistA,2)-powf(DistB,2))/(-2*DistA*DistB)));  //get positive angle (from robot - to target)
    MTX = SessionDriveX - x1;                                                                 //move x1 to origin (kinda)
    MTY = SessionDriveY - y1;                                                                 //move y1 to origin
    RotatedY = -MTX*sin(Rads(h1))+MTY*cos(Rads(h1));                                          //get rotated Y value
    //SessionTurn = SessionTurn * -((RotatedY)/std::abs(RotatedY));                             //make SessionTurn the proper value

    SessionTurn = SessionTurn * ((RotatedY)/std::abs(RotatedY));                             //make SessionTurn the proper value
    float thisThing = 1;
    Error = (sqrtf(powf(x1-SessionDriveX, 2) + powf(y1-SessionDriveY, 2))*thisThing - SessionDriveRadius*thisThing)*(sqrtf(powf(x1-SessionDriveX, 2) + powf(y1-SessionDriveY, 2))*thisThing - SessionDriveRadius*thisThing);

    if(LimitSwitchFront || LimitSwitchBack){
      if(Error < 5){
        Error = 5;//this keeps the pid moving for longer than usual when trying to grab a goal.
      }
    }
    SmartVoltage = GetClosestToZero(Error, Ramp * (Error/std::abs(Error)));                   //this is to keep wheels from slipping, ultimately resulting in a faster turn.

    Integral = Integral + Error;
    Derivative = Error - PreviousError;
    if (Error == 0) {Integral = 0;} //these are to prevent the integral from getting too large
    if (Error > 1) {Integral = 0;}
    if (std::abs(Error) > std::abs(SessionMaxSpeed/12)) {Integral = 0;}

    Ramp += 10;

    OverallSpeed = SmartVoltage * DKp + Integral * DKi + Derivative * DKd;                                                                   //* TKp + Integral * TKi + Derivative * TKd;
    OverallSpeed = GetClosestToZero(OverallSpeed, SessionMaxSpeed * (Error/std::abs(Error)));

    if(std::abs(OverallSpeed)<MinVoltage){
      OverallSpeed = MinVoltage * (std::abs(OverallSpeed)/OverallSpeed);
    }

    PreviousError = Error;

    RightSpeed = SessionTurn * std::abs(Error/500);

    if(std::abs(OverallSpeed) > 12){
      OverallSpeed = 12 * std::abs(OverallSpeed)/OverallSpeed;
    }

    if(std::abs(RightSpeed) > 4){
      RightSpeed = 4 * (std::abs(RightSpeed)/RightSpeed);
    }    


    if((FrontSensorsSenseATouch == true) && LimitSwitchFront) {
      frontAir.set(true);
      Condition=false;
    } else if((LimSwitchBack.pressing()) && LimitSwitchBack) {
      backAir.set(true);
      Condition=false;
    }

    //PositiveDiagonalVoltage = sin(Rads(SessionTurn+45))*std::abs(Voltage);
    //NegativeDiagonalVoltage = -sin(Rads(SessionTurn-45))*std::abs(Voltage);

    FLMotor.spin(forward, (OverallSpeed - RightSpeed)*DirectionalFactor, voltageUnits::volt);
    FRMotor.spin(forward, (OverallSpeed + RightSpeed)*DirectionalFactor, voltageUnits::volt);
    BRMotor.spin(forward, (OverallSpeed + RightSpeed)*DirectionalFactor, voltageUnits::volt);
    BLMotor.spin(forward, (OverallSpeed - RightSpeed)*DirectionalFactor, voltageUnits::volt);


    if (std::abs(Error)-4 <= 0.0 || ReachedTarget || Brain.Timer.systemHighResolution() - StartTime > SessionTimeout) {
      if(!(ReachedTarget)){
        ReachedTargetTime = Brain.Timer.systemHighResolution();
      }

      ReachedTarget = true;

      if(Brain.Timer.systemHighResolution() - ReachedTargetTime >= 250000 || Brain.Timer.systemHighResolution() - StartTime > SessionTimeout){
        break;
      }
    }

    task::yield();

  }  

  Drivetrain(stop(brake);)

  PIDsRunning --;


  return 0;

}

void DriveTo(float Drive_x, float Drive_y, float speed, float radius, bool Wait_, float Coustom_Timeout, bool _DriveForwards){

  Speed_V = (speed/100)*12;

  DriveX = Drive_x;
  DriveY = Drive_y;
  CoustomTimeout = Coustom_Timeout * 1000000;
  DriveForwards = _DriveForwards;

  DriveRadius = radius;

  Wait = Wait_;

  wait(20, msec);

  if (Wait) {

    _DriveTo_();

  } else {

    PID = task(_DriveTo_);

  }

  wait(20, msec);

}

int _Turn_To_() {

  /*
  turns the robot in degrees
  */

  float SessionTimeout = CoustomTimeout;
  float SessionTurnX = TurnX;
  float SessionTurnY = TurnY;
  float SessionMaxSpeed = Speed_V;
  float SessionTurnDegree = TurnDegree;

  //float SessionTurn = ((( DriveCircumfrence * ( TurnDistance / 360 )) / ( WheelDiameter * 3.14 )) / DriveGearRatio ) *360;
  bool SessionWait = Wait;

  PIDsRunning ++;

  while(PIDsRunning > 1){
    if (SessionWait) {
      wait(20, msec);
    } else {
      task::yield();
    }
  }

  float StartTime = Brain.Timer.systemHighResolution();

  float AverageX = 0;
  float AverageY = 0;
  float AverageH = 0;

  int SampleSize = 0;

  while(Brain.Timer.systemHighResolution() - StartTime < 1000000){
    AverageX += GpsX;
    AverageY += GpsY;
    AverageH += GpsH + SessionTurnDegree;
    SampleSize += 1;
    task::yield();
  }

  float x1 = AverageX / SampleSize;
  float y1 = AverageY / SampleSize;
  float h1 = AverageH / SampleSize;

  float x2 = x1 + cos(Rads(h1));
  float y2 = y1 + sin(Rads(h1));

  float DistA = 1.0;
  float DistB = sqrt(powf(x1-SessionTurnX,2)+powf(y1-SessionTurnY,2));
  float DistC = sqrt(powf(x2-SessionTurnX,2)+powf(y2-SessionTurnY,2));

  float SessionTurn = Degs(acosf((powf(DistC,2)-powf(DistA,2)-powf(DistB,2))/(-2*DistA*DistB)));

  float MTX = SessionTurnX - x1;
  float MTY = SessionTurnY - y1;

  float RotatedY = -MTX*sin(Rads(h1))+MTY*cos(Rads(h1));

  SessionTurn = SessionTurn * -((RotatedY)/std::abs(RotatedY));

  Brain.Screen.print(SessionTurn);
  Brain.Screen.setCursor(2,1);

  SetDriveBrake(brake);

  FLMotor.setPosition(0, degrees);
  FRMotor.setPosition(0, degrees);
  BLMotor.setPosition(0, degrees);
  BRMotor.setPosition(0, degrees);

  bool Condition = true;
  bool ReachedTarget = false;

  float ReachedTargetTime = 0;

  float Error = 0;
  float Integral = 0;
  float PreviousError = 0;
  float Derivative;
  float Voltage;
  float SmartError;
  int Ramp = 0;
  float ActualVoltage = 0;
  float Multiplier = 4.5;

  if(FrontSensorsSenseATouch && !(LimSwitchBack.pressing())) {
    Multiplier +=2;
  }
  if(!(FrontSensorsSenseATouch) && (LimSwitchBack.pressing())) {
    Multiplier +=5.1;
  }
  if(LimSwitchBack.pressing() && FrontSensorsSenseATouch){
    Multiplier +=4;
  }

  while (Condition) {

    x1 = GpsX;
    y1 = GpsY;
    h1 = GpsH + SessionTurnDegree;
    x2 = x1 + cos(Rads(h1));
    y2 = y1 + sin(Rads(h1));
    DistB = sqrt(powf(x1-SessionTurnX,2)+powf(y1-SessionTurnY,2));
    DistC = sqrt(powf(x2-SessionTurnX,2)+powf(y2-SessionTurnY,2));
    SessionTurn = Degs(acosf((powf(DistC,2)-powf(DistA,2)-powf(DistB,2))/(-2*DistA*DistB)));
    MTX = SessionTurnX - x1;
    MTY = SessionTurnY - y1;
    RotatedY = -MTX*sin(Rads(h1))+MTY*cos(Rads(h1));
    SessionTurn = SessionTurn * -((RotatedY)/std::abs(RotatedY));

    Error = SessionTurn;
    // Error = (1/(1+ powf(2.718, -(SessionTurn)/9.5))) * (SessionMaxSpeed*2) - SessionMaxSpeed;

    Integral = Integral + Error;
    if(Integral > 40)(Integral = Error);
    Derivative = Error - PreviousError;
    if (Error == 0) {Integral = 0;} //these are to prevent the integral from getting too large
    if (std::abs(Voltage) > 5) {Integral = 0;}
    //if (std::abs(Error) > std::abs(SessionMaxSpeed/12)) {Integral = 0;}

    Ramp += 15;
    PreviousError = Error;

    SmartError = GetClosestToZero(Error, Ramp * (Error/std::abs(Error)));

    Voltage = SmartError * TKp * Multiplier + Integral * TKi + Derivative * TKd;
    Voltage = GetClosestToZero(Voltage, SessionMaxSpeed * (Error/std::abs(Error)));

    if(std::abs(Voltage)<MinVoltage){
      Voltage = MinVoltage * (std::abs(Voltage)/Voltage);
    }

    if (ActualVoltage < Voltage){ActualVoltage = ActualVoltage + 1;}
    if (ActualVoltage > Voltage){ActualVoltage = ActualVoltage - 1;}

    FLMotor.spin(forward, ActualVoltage, voltageUnits::volt);
    FRMotor.spin(reverse, ActualVoltage, voltageUnits::volt);
    BRMotor.spin(reverse, ActualVoltage, voltageUnits::volt);
    BLMotor.spin(forward, ActualVoltage, voltageUnits::volt);

    

    if (std::abs(SessionTurn)-.5 <= 0 || ReachedTarget || Brain.Timer.systemHighResolution() - StartTime > SessionTimeout) {
      if(!(ReachedTarget)){
        ReachedTargetTime = Brain.Timer.systemHighResolution();
      }

      ReachedTarget = true;

      if(Brain.Timer.systemHighResolution() - ReachedTargetTime >= 250000){
        break;
      }
    }

    if (SessionWait) {
      wait(20, msec);
    } else {
      task::yield();
    }

  }
  

  FLMotor.stop();
  FRMotor.stop();
  BRMotor.stop();
  BLMotor.stop();

  PIDsRunning --;


  return 0;

}

void TurnTo(float Turn_x, float Turn_y, float speed, bool Wait_, float Turn_Degree, float Coustom_Timeout) {

  Speed_V = (speed/100)*12;

  TurnDegree = Turn_Degree;
  CoustomTimeout = Coustom_Timeout * 1000000;
  TurnX = Turn_x;
  TurnY = Turn_y;
  Wait = Wait_;

  wait(20, msec);

  if (Wait) {

    _Turn_To_();

  } else {

    PID = task(_Turn_To_);

  }

  wait(20, msec);

}

int _Turn_() {


  /*
  turns the robot in degrees
  */
  float SessionTimeout = CoustomTimeout;
  float SessionTurn = TurnDistance;
  float SessionSpeed = Speed_V;

  //float SessionTurn = ((( DriveCircumfrence * ( TurnDistance / 360 )) / ( WheelDiameter * 3.14 )) / DriveGearRatio ) *360;
  bool SessionWait = Wait;

  PIDsRunning ++;

  while(PIDsRunning > 1){
    if (SessionWait) {
      wait(20, msec);
    } else {
      task::yield();
    }
  }

  Inertial.setRotation(0, deg);

  Brain.Timer.reset();

  SetDriveBrake(coast);

  FLMotor.setPosition(0, degrees);
  FRMotor.setPosition(0, degrees);
  BLMotor.setPosition(0, degrees);
  BRMotor.setPosition(0, degrees);

  bool Condition = true;

  float Error;
  float Integral = 0;
  float PreviousError = 0;
  float Derivative;
  float Speed;
  float smartError;
  int x = 0;
  float avgm;
  bool ReachedTarget = false;
  bool ReachedTargetTime = 0;
  while (Condition) {

    //avgm = std::abs((FrontLeftDrive.position(degrees) - FrontRightDrive.position(degrees) + BackLeftDrive.position(degrees) -     BackRightDrive.position(degrees)) / 4);
    avgm = std::abs(Inertial.rotation(degrees));

    Error = std::abs(SessionTurn) - avgm;
    Integral = Integral + Error;

    if (Error <= x) {
      smartError = Error;
    } else if (x <= Error) {
       smartError = x;
     }

    if (Error == 0) {
      Integral = 0;
    }

    if (std::abs(Error) > 5 || FLMotor.velocity(percent) > 25) {
      Integral = 0;
    }

    //if(Integral > 12){
    //  Integral = 12;
    //}

    Derivative = Error - PreviousError;
    PreviousError = Error;

    Speed = smartError * TKp + Integral * TKi + Derivative * TKd;

    x += 50;

    if (Speed > SessionSpeed) {
      Speed = SessionSpeed;
    }

    if (SessionTurn > 0) {
    FLMotor.spin(forward, Speed, voltageUnits::volt);
    FRMotor.spin(reverse, Speed, voltageUnits::volt);
    BRMotor.spin(reverse, Speed, voltageUnits::volt);
    BLMotor.spin(forward, Speed, voltageUnits::volt);
    } else {
      FLMotor.spin(reverse, Speed, voltageUnits::volt);
      FRMotor.spin(forward, Speed, voltageUnits::volt);
      BRMotor.spin(forward, Speed, voltageUnits::volt);
      BLMotor.spin(reverse, Speed, voltageUnits::volt);
    }

    if (std::abs(avgm) > std::abs(SessionTurn) - 1.5 || Brain.Timer.value() > SessionTimeout) { //was 40

      if(!ReachedTarget){
        ReachedTarget = true;
        ReachedTargetTime = Brain.Timer.systemHighResolution();
      }

      if(Brain.Timer.systemHighResolution() - ReachedTargetTime >= 500000 || Brain.Timer.value() > SessionTimeout){
        //break;
      }

    }

    if (SessionWait) {
      wait(20, msec);
    } else {
      task::yield();
    }

  }
  

  FLMotor.stop(brake);
  FRMotor.stop(brake);
  BRMotor.stop(brake);
  BLMotor.stop(brake);

  PIDsRunning --;


  return 0;

}

void Turn(float Turn_, float speed, bool Wait_, float CoustomTimeout_) {

  Speed_V = (speed/100)*12;

  CoustomTimeout = CoustomTimeout_;
  TurnDistance = Turn_;
  Wait = Wait_;

  wait(20, msec);

  if (Wait) {

    _Turn_();

  } else {

    TURN_PID = task(_Turn_);

  }

  wait(20, msec);

}

void TurnAndDrive(float x_point, float y_point, float driveSpeed, float turnSpeed, float radius, bool faceDirection, bool driveWait, float Turn_Timeout, float Drive_Timeout) {
  
  float Direction;
  
  if(faceDirection == true){
    Direction = 0;
    DriveForwards = true;
  } else {
    Direction = 180;
    DriveForwards = false;
  }

  TurnTo(x_point, y_point, turnSpeed, true, Direction, Turn_Timeout);
  DriveTo(x_point, y_point, driveSpeed, radius, driveWait, Drive_Timeout, DriveForwards);
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
    
    if((RSD < 1.6 && LSD < 1.6) && (DistanceRight.isObjectDetected() && DistanceLeft.isObjectDetected())){
      FrontSensorsSenseATouch = true;
    }else{
      FrontSensorsSenseATouch = false;
    }

    Brain.Screen.clearScreen();
    Brain.Screen.print(FrontSensorsSenseATouch);

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