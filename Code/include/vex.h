/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       vex.h                                                     */
/*    Author:       Vex Robotics                                              */
/*    Created:      1 Feb 2019                                                */
/*    Description:  Default header for V5 projects                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/
//
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>

#include "v5.h"
#include "v5_vcs.h"

#include "robot-config.h"
#include "functions.h"
#include "chassi.h"
#include "preauton.h"

#define WaitUntil(condition)                                                   \
  do {                                                                         \
    task::sleep(20);                                                      \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)