  #include "vex.h"
void FrontLiftt(float target, bool wait, float speed){

  float FrontLiftPos = FrontLift.position(degrees);
  target = target/100.0 * 580.0;
  float amount = target - FrontLiftPos;

  FrontLift.setVelocity(speed,percent);

  FrontLift.spinFor(forward, amount, degrees, wait);

}

void BackLiftt(float target, bool wait, float speed){
  
  float BackLiftPos = BackLift.position(degrees);
  target = target/100.0 * 580.0;
  float amount = target - BackLiftPos;

  BackLift.setVelocity(speed,percent);

  BackLift.spinFor(forward, amount, degrees, wait);

}

