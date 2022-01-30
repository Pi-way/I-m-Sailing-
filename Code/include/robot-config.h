using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor FRMotor;
extern motor FLMotor;
extern motor BRMotor;
extern motor BLMotor;
extern motor RingLiftL;
extern motor RingLiftR;
extern controller Controller1;
extern controller Controller2;
extern gps GPSLeft;
extern gps GPSRight;
extern motor FrontLift;
extern inertial Inertial;
extern motor BackLift;
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