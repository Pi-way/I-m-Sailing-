  #include "vex.h"

float Amount;
float Speed;
float Waittt;
task RingTHING;
int RingThingsRunning = 0;

void FrontLiftt(float target, bool Waitttt, float speed){

  float FrontLiftPos = FrontLift.position(degrees);
  target = target/100.0 * 625.0;
  float amount = target - FrontLiftPos;

  FrontLift.setVelocity(speed,percent);

  FrontLift.spinFor(forward, amount, degrees, Waitttt);

}

void BackLiftt(float target, bool Waitttt, float speed){
  
  float BackLiftPos = BackLift.position(degrees);
  target = target/100.0 * 590.0;
  float amount = target - BackLiftPos;

  BackLift.setVelocity(speed,percent);

  BackLift.spinFor(forward, amount, degrees, Waitttt);

}

int Please(){

  RingThingsRunning ++;

  while(RingThingsRunning > 1){
    task::yield();
  }

  RingLiftL.setVelocity(Speed,percent);
  RingLiftR.setVelocity(Speed,percent);

  RingLiftL.spin(forward);
  RingLiftR.spin(forward);

  RingLiftL.setPosition(0,degrees);

  while(true){//RingLiftL.position(degrees) < Amount * 360
    while(std::abs(RingLiftL.velocity(rpm)) > Speed / 50 && RingLiftL.position(degrees) < Amount * 360){
      RingLiftL.setVelocity(Speed,rpm);
      RingLiftR.setVelocity(Speed,rpm);
    }
    while(std::abs(RingLiftL.velocity(rpm)) < Speed / 50 && RingLiftL.position(degrees) < Amount * 360){
      RingLiftL.setVelocity(-100,pct);
      RingLiftR.setVelocity(-100,pct);
    }
  }

  RingLiftL.setVelocity(0,percent);
  RingLiftR.setVelocity(0,percent);

  RingThingsRunning -= 1;
}

void RingThing(float speed, float amount, bool Waitttt){
  Speed = speed;
  Amount = amount;
  if(Waitttt){
    Please();
  }else{
    RingTHING = task(Please);
  }

}



