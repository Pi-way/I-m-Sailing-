/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       functions.h                                               */
/*    Author:       Team 98548A                                               */
/*    Created:      8/20/2021                                                 */
/*    Description:  Header file for functions.cpp                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

void BackLiftt(float target, bool wait_for_completion = true, float speed = 100);
void FrontLiftt(float target, bool wait_for_completion = true, float speed = 100);
void StartRingThing(float speed = 400, float amount = 10000000, bool wait_for_completion = false);
void StopRingThing();
void autoPark(bool = true);