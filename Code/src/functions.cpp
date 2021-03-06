#include "vex.h"

float Amount;
float Speed;
float Waittt;
task RingTHING;
int RingThingsRunning = 0;

bool RTS = false;

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
  RTS = false;

  while(RingThingsRunning > 1){
    task::yield();
  }

  RingLiftL.setVelocity(Speed,percent);
  RingLiftR.setVelocity(Speed,percent);

  RingLiftL.spin(forward);
  RingLiftR.spin(forward);

  RingLiftL.setPosition(0,degrees);

  while(!RTS){//RingLiftL.position(degrees) < Amount * 360
    RingLiftL.setVelocity(Speed,rpm);
    RingLiftR.setVelocity(Speed,rpm);
    wait(.25,sec);
    while(std::abs(RingLiftL.velocity(rpm)) > 0.65 * Speed && RingLiftL.position(degrees) < Amount * 360 && !RTS){
      task::yield();
    }
    RingLiftL.setVelocity(-100,pct);
    RingLiftR.setVelocity(-100,pct);
    wait(.125,sec);
    while(std::abs(RingLiftL.velocity(rpm)) < Speed / 2 && RingLiftL.position(degrees) < Amount * 360 && !RTS){
      task::yield();
    }

    
  }

  RingLiftL.setVelocity(0,percent);
  RingLiftR.setVelocity(0,percent);

  RingThingsRunning -= 1;
  RTS = false;

  return 0;
}

void StartRingThing(float speed, float amount, bool Waitttt){
  Speed = speed;
  Amount = amount;
  if(Waitttt){
    Please();
  }else{
    RingTHING = task(Please);
  }

}

void StopRingThing(){
  RTS = true;
}

void spinMotors(float speed){
  FLMotor.spin(forward,speed,pct);
  FRMotor.spin(forward,speed,pct);
  BRMotor.spin(forward,speed,pct);
  BLMotor.spin(forward,speed,pct);
}

void stopMotors(){
  FLMotor.stop();
  FRMotor.stop();
  BRMotor.stop();
  BLMotor.stop();
}

void autoPark(bool forward){
  
}
