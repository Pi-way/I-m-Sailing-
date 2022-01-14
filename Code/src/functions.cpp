  #include "vex.h"
void FrontLiftt(float target, bool waitt, float speed){

  float FrontLiftPos = FrontLift.position(degrees);
  target = target/100.0 * 625.0;
  float amount = target - FrontLiftPos;

  FrontLift.setVelocity(speed,percent);

  FrontLift.spinFor(forward, amount, degrees, waitt);

}

void BackLiftt(float target, bool waitt, float speed){
  
  float BackLiftPos = BackLift.position(degrees);
  target = target/100.0 * 590.0;
  float amount = target - BackLiftPos;

  BackLift.setVelocity(speed,percent);

  BackLift.spinFor(forward, amount, degrees, waitt);

}

void RingThing(float speed, float amount, bool waitt){
  RingLiftL.setVelocity(speed,percent);
  RingLiftR.setVelocity(speed,percent);

  RingLiftL.spinFor(forward,amount*360,degrees,false);
  RingLiftR.spinFor(forward,amount*360,degrees,waitt);
}

