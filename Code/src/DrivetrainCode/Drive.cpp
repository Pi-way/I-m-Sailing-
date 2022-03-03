#include "vex.h"

float Kp = 0.1;
float Ki = 0.1;
float Kd = 0.05;

int _Drive_() {
  float SessionDistance = ((Distance/12.566))*360;
  bool SessionWait = Wait;
  float SessionSpeed = Speed;
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

    if(Error < ((3/12.566))*360 && (FrontButton || BackButton)){ //If error is less than ~3 inch AND you are waiting for a goal to touch
      if (std::abs(avgm) > std::abs(((SessionMaxDistance/12.566))*360)){ //If you have gone too far
        STAHP = true;
      }else{ //you haven't reached the goal and haven't gone too far.
        Error = ((3/12.566))*360; 
      }
    }

    Integral = Integral + Error;
    Derivative = Error - PreviousError;
    PreviousError = Error;
    Ramp += 4;

    if (Error <= Ramp) { smartError = Error; }
    else if (Ramp <= Error) { smartError = Ramp; }
    if (Error == 0) { Integral = 0; }
    if (std::abs(Error) > SessionSpeed ) { Integral = 0; }

    Speed = smartError * Kp + Integral * Ki + Derivative * Kd;

    if (Speed > SessionSpeed) { Speed = SessionSpeed; }

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

void Drive(float Distance_, float Speed_, bool Wait_, bool f_b, bool b_b, bool coast, float maxDistance) {

  Distance = Distance_;
  Wait = Wait_;
  Speed = (Speed_/100)*12;
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