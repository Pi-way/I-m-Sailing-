/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Module: functions.cpp                                                      */
/* Author: Team 98548A                                                        */
/* Created: 8/20/2021                                                         */
/* Description: Contains functions for autonomouses                           */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
//regular variables to specify the amount we want to do something and the speed to do it at
float Amount;
float Speed;

//variables and objects for the ring lift
task RingLiftTask;
int RingThingsRunning = 0;
bool _StopRingThing_ = false;

//function to move the front lift to a specific location
void FrontLiftt(float target, bool wait, float speed){

	float FrontLiftPos = FrontLift.position(degrees);
	target = target/100.0 * 625.0;
	float amount = target - FrontLiftPos;

	FrontLift.setVelocity(speed,percent);
	FrontLift.spinFor(forward, amount, degrees, wait);

}

//function to move the back lift to a specific location
void BackLiftt(float target, bool wait, float speed){

	float BackLiftPos = BackLift.position(degrees);
	target = target/100.0 * 590.0;
	float amount = target - BackLiftPos;

	BackLift.setVelocity(speed,percent);
	BackLift.spinFor(forward, amount, degrees, wait);

}

//base function to controll the ring lift
int _StartRingThing_(){
	
	//logic to prevent multiple tasks from running at the same time
	RingThingsRunning ++;
	while(RingThingsRunning > 1){
		task::yield();
	}

	//set the global override variable to false
	_StopRingThing_ = false;
	//set motor positions and velocities
	RingLiftL.setVelocity(Speed,percent);
	RingLiftR.setVelocity(Speed,percent);
	RingLiftL.spin(forward);
	RingLiftR.spin(forward);
	RingLiftL.setPosition(0,degrees);

	//main loop for ring lift	
	while(!_StopRingThing_){

		//start the ring lift and wait for it to get up to speed for 1/4 of a second
		RingLiftL.setVelocity(Speed,rpm);
		RingLiftR.setVelocity(Speed,rpm);
		wait(.25,sec);

		//loop while the ring lift's speed is above 65%, the lift hasn't spun too much, and the global override isset to false
		while(std::abs(RingLiftL.velocity(rpm)) > 0.65 * Speed && RingLiftL.position(degrees) < Amount * 360 &&!_StopRingThing_){
			task::yield();
		}

		//reverse the ring lift for at least 1/8 of a second
		RingLiftL.setVelocity(-100,pct);
		RingLiftR.setVelocity(-100,pct);
		wait(.125,sec);

		//loop while the ring lift is jammed, the lift hasn't spun too much, and the global override is set to false
		while(std::abs(RingLiftL.velocity(rpm)) < Speed / 2 && RingLiftL.position(degrees) < Amount * 360 &&!_StopRingThing_){
			task::yield();
		}	
	}
	
	//stop the ring lift
	RingLiftL.setVelocity(0,percent);
	RingLiftR.setVelocity(0,percent);

	//let other tasks know they can run by subtracting 1 from the number of tasks running
	RingThingsRunning -= 1;
	_StopRingThing_ = false;

	return 0;
}
 //main ring lift function
void StartRingThing(float speed, float amount, bool wait){

	//set global variables to desired values
	Speed = speed;
	Amount = amount;

	//determines if the ring lift function should run as a task Por a regular function
	if(wait){
		_StartRingThing_();
	}else{
		RingLiftTask = task(_StartRingThing_);
	}
	
}

//this function sets a global variable to false which will stop the ring lift no matter what
void StopRingThing(){
	_StopRingThing_ = true;
}