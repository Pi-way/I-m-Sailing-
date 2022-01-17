#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors

SmartMotor FRMotor = SmartMotor(PORT2, ratio18_1, false);
SmartMotor FLMotor = SmartMotor(PORT20, ratio18_1, true);
SmartMotor BRMotor = SmartMotor(PORT3, ratio18_1, false);
SmartMotor BLMotor = SmartMotor(PORT8, ratio18_1, true);
SmartMotor RingLiftL = SmartMotor(PORT13, ratio6_1, true);
SmartMotor RingLiftR = SmartMotor(PORT14, ratio6_1, false);
controller Controller1 = controller(primary);
controller Controller2 = controller(partner);
SmartMotor FrontLift = SmartMotor(PORT15, ratio36_1, true);
gps GPS5 = gps(PORT5, -2.77, -4.80, inches, 178); //vALUES FOR LEFT FIELD: gps GPS5 = gps(PORT5, 2.7, 5.60, inches, -3); (SLOT 8) ///////add x to make it smaller and add y to make it smaller 
inertial Inertial = inertial(PORT18);
SmartMotor BackLift = SmartMotor(PORT10, ratio36_1, false);
digital_out frontAir = digital_out(Brain.ThreeWirePort.G);
digital_out backAir = digital_out(Brain.ThreeWirePort.E);
digital_out tilt = digital_out(Brain.ThreeWirePort.H);
limit LimSwitchBack = limit(Brain.ThreeWirePort.F);
distance DistanceRight = distance(PORT7);
distance DistanceLeft = distance(PORT16);


// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}