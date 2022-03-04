/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       DrivetrianStuff.h                                         */
/*    Author:       Team 98548A                                               */
/*    Created:      8/20/2021                                                 */
/*    Description:  Header file for DrivetrainCode and other dependendents    */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// This is a preprocessor macro that allows us to set all of our drivetrain motor's brake type in one line.
#define SetDriveBrake(brake_mode)                     \
  BRMotor.setStopping(brake_mode);                    \
  BLMotor.setStopping(brake_mode);                    \
  FRMotor.setStopping(brake_mode);                    \
  FLMotor.setStopping(brake_mode);

// This is a preprocessor macro that allows us to reset all of our drivetrain motor's positions in one line.
#define SetDrivePosition(DrivePosition)               \
  BRMotor.setPosition((DrivePosition), degrees);      \
  BLMotor.setPosition((DrivePosition), degrees);      \
  FRMotor.setPosition((DrivePosition), degrees);      \
  FLMotor.setPosition((DrivePosition), degrees);

// This is a preprocessor macro that allows us to call any member function of each drivetrain motor in one line.
#define Drivetrain( MotorMember )                     \
  BRMotor.MotorMember                                 \
  BLMotor.MotorMember                                 \
  FRMotor.MotorMember                                 \
  FLMotor.MotorMember

// These two macros allow us to convert between Radians and Degrees.
#define Rads(Degs) ((3.1415926535/180.0)*Degs)
#define Degs(Rads) ((180.0/3.1415926535)*Rads)

// This macro represents the minimum amount of voltage required to move the robot. (used as a part of our PID)
#define MinVoltage 1.5

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
extern float Speed;
extern float TurnDegree;
extern float TurnDistance;
extern float CoustomTimeout;
extern bool ExpectFrontButton;
extern bool ExpectBackButton;
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

// This is a function prototype that returns the number closest to zero (even negative numbers).
float GetClosestToZero(float First, float Second);

// Function prototype for calibrate
void Calibrate();

// Function prototype for StartTasks
void StartTasks();

// This function allows the robot to turn using a PID and our inertial sensor. (other features will be explained in notebook)
void Turn(float amount, float speed = 100, bool wait = true, float coustom_timeout = 2.0);

// This function allows the robot to turn to a certain point using a PID in combination with our GPS sensors. (other features will be explained in notebook)
void TurnTo(float x_cooridnate, float t_coordinate, float speed = 100, bool wait = true, float turn_offset = 0, float coustom_timeout = 2.5);

// This function allows the robot to drive using a PID in combination with the rotation encoders found in our drivetrain motors. (other features will be explained in notebook)
void Drive(float distance, float speed = 100, bool wait = true, bool expect_front_button = false, bool expect_back_button = false, bool coast = false, float max_distance = 10000000);

// This function does essentially the same thing as Drive, only faster and with less precision. (other features will be explained in notebook)
void DriveFast(float distance, float speed = 100, bool wait = true, bool expect_front_button = false, bool expect_back_button = false, bool coast = false, float max_distance = 10000000);

// This function allows the robot to drive to a point using a PID in combination with our GPS sensors. (other features will be explained in notebook)
void DriveTo(float drive_x, float drive_y, float speed, float drive_radius = 0, bool wait = true, float coustom_timeout = 5);

// This function combines the functionality of TurnTo() and DriveTo(). (other features will be explained in notebook)
void TurnAndDrive(float x_point, float y_point, float drive_speed = 80, float turn_speed = 80, float drive_radius = 0, bool face_direction = true, bool drive_wait = true, float turn_timeout = 2.5, float drive_timeout = 5);