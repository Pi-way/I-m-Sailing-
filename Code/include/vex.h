/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       vex.h                                                     */
/*    Author:       Caleb Carlson, Team 98548A                                */
/*    Created:      8/20/2021                                                 */
/*    Description:  Default header for V5 projects                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>

#include "v5.h"
#include "v5_vcs.h"

#include "robot-config.h"
#include "functions.h"
#include "DrivetrainStuff.h"
#include "preauton.h"

#define WaitUntil(condition)  \
  do {                        \
    task::yield();            \
  } while (!(condition))

#define repeat(iterations)    \
  for (int iterator = 0; iterator < iterations; iterator++)