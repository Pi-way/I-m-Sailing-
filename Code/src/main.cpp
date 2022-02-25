/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       vex.h                                                     */
/*    Author:       Caleb Carlson, Team 98548A                                */
/*    Created:      8/20/2021                                                 */
/*    Description:  Main source code file                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
//dum
using namespace vex;

competition Competition;

void pre_auton(void);
void autonomous(void);
void usercontrol(void);

int main() {


  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  while (true) {
    wait(100, msec);
  }
  backAir.set(true);
  frontAir.set(true);

}

