/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       vex.h                                                     */
/*    Author:       Team 98548A                                               */
/*    Created:      8/20/2021                                                 */
/*    Description:  Header file for DrivetrainCode and other dependendents    */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// Declare vex::task objects for GPS, and declare corrisponding global variables.
extern vex::task Gps_X; extern float GpsX;
extern vex::task Gps_Y; extern float GpsY;
extern vex::task Gps_H; extern float GpsH;

// Declare a vex::task object that will print GPS values to the controller screen.
extern vex::task ControllerGPS;

// Declare a vex::task object that will run any PID loops for drivetrain.
extern vex::task PID;

// Declare a vex::task object to run the logic that allows the sensors on the front lift to be used.
extern vex::task FrontLiftSensorsTask;

// Declare a boolean variable that represents when the robot thinks a goal (or something) is within clamping reach of our pneumatic claws. This variable is continually updated via the FrontLiftSensorsTask.
extern bool FrontSensorsSenseATouch;

// Declare an integer variable that represents the number of active PIDs. This variable is utilitized by each PID controller to ensure they don't run at the same time.
extern int PIDsRunning;

// Variables below are global variables used as a wrap-around to the fact that vex::task doesn't support passing variables into the function they run.
extern float Distance; 
extern float Speed_V;
extern float TurnDegree;
extern float TurnDistance;
extern float CoustomTimeout;
extern bool fb;
extern bool bb;
extern bool Coast;
extern float MaxDistance;
extern float DriveRadius;
extern float DriveX;
extern float DriveY;
extern float TurnX;
extern float TurnY;
extern bool Wait;

// Declare the drive balance variable: this variable is used to compensate for any discrepancy in drivetrain friction.
extern float Drive_balance;

#define SetDriveBrake(brake_mode)                     \
  BRMotor.setStopping(brake_mode);                    \
  BLMotor.setStopping(brake_mode);                    \
  FRMotor.setStopping(brake_mode);                    \
  FLMotor.setStopping(brake_mode);

#define SetDrivePosition(DrivePosition)               \
  BRMotor.setPosition((DrivePosition), degrees);      \
  BLMotor.setPosition((DrivePosition), degrees);      \
  FRMotor.setPosition((DrivePosition), degrees);      \
  FLMotor.setPosition((DrivePosition), degrees);

#define Drivetrain( MotorMember )                     \
  BRMotor.MotorMember                                 \
  BLMotor.MotorMember                                 \
  FRMotor.MotorMember                                 \
  FLMotor.MotorMember

#define Rads(Degs) ((3.1415926535/180.0)*Degs)
#define Degs(Rads) ((180.0/3.1415926535)*Rads)

#define MinVoltage 1.5

float GetClosestToZero(float First, float Second);

void Calibrate();
void StartTasks();

void Turn(float Turn_, float speed = 100, bool Wait_ = true, float CoustomTimeout_ = 2.0);
void TurnTo(float Turn_x, float Turn_y, float speed = 100, bool Wait_ = true, float Turn_Degree = 0, float Coustom_Timeout = 2.5);
void Drive(float Distance_, float Speed_V_ = 100, bool Wait_ = true, bool f_b = false, bool b_b = false, bool coast = false, float maxDistance = 10000000);
void DriveFast(float Distance_, float Speed_V_ = 100, bool Wait_ = true, bool f_b = false, bool b_b = false, bool coast = false, float maxDistance = 10000000);
void DriveTo(float Drive_x, float Drive_y, float speed, float radius = 0, bool Wait_ = true, float Coustom_Timeout = 5);
void TurnAndDrive(float x_point, float y_point, float driveSpeed = 80, float turnSpeed = 80, float radius = 0, bool faceDirection = true, bool driveWait = true, float TurnTimeout = 2.5, float DriveTimeout = 5);