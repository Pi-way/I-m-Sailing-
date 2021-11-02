#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor FRMotor = motor(PORT8, ratio6_1, false);
motor FLMotor = motor(PORT12, ratio6_1, true);
motor BRMotor = motor(PORT9, ratio6_1, false);
motor BLMotor = motor(PORT11, ratio6_1, true);
controller Controller1 = controller(primary);
motor FrontLift = motor(PORT6, ratio36_1, true);
gps GPS5 = gps(PORT5, 2.73, 5.36, inches, -3);
inertial Inertial = inertial(PORT4);
motor BackLift = motor(PORT1, ratio36_1, false);
digital_out frontAir = digital_out(Brain.ThreeWirePort.A);
digital_out backAir = digital_out(Brain.ThreeWirePort.G);

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