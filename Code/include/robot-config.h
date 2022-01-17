using namespace vex;
#include "CoustomMotorClass.h"

extern brain Brain;

// VEXcode devices
extern SmartMotor FRMotor;
extern SmartMotor FLMotor;
extern SmartMotor BRMotor;
extern SmartMotor BLMotor;
extern SmartMotor RingLiftL;
extern SmartMotor RingLiftR;
extern controller Controller1;
extern controller Controller2;
extern gps GPS5;
extern SmartMotor FrontLift;
extern inertial Inertial;
extern SmartMotor BackLift;
extern digital_out frontAir;
extern digital_out backAir;
extern digital_out tilt;
extern limit LimSwitchBack;
extern distance DistanceRight;
extern distance DistanceLeft;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );