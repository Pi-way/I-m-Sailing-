
extern vex::task Gps_X; extern float GpsX;
extern vex::task Gps_Y; extern float GpsY;
extern vex::task Gps_H; extern float GpsH;
extern vex::task PID;
extern vex::task ControllerGPS;
extern vex::task FrontLiftSensorsTask;
extern bool FrontSensorsSenseATouch;
extern bool Calibrated;
extern int PIDsRunning;
extern float Distance;
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
extern float Speed_V;
extern float Drive_balance;



#define SetDriveBrake(brake_mode)       \
  BRMotor.setStopping(brake_mode);      \
  BLMotor.setStopping(brake_mode);      \
  FRMotor.setStopping(brake_mode);      \
  FLMotor.setStopping(brake_mode);

#define SetDrivePosition(DrivePosition)               \
  BRMotor.setPosition((DrivePosition), degrees);      \
  BLMotor.setPosition((DrivePosition), degrees);      \
  FRMotor.setPosition((DrivePosition), degrees);      \
  FLMotor.setPosition((DrivePosition), degrees);

#define Drivetrain( MotorMember )            \
  BRMotor.MotorMember                      \
  BLMotor.MotorMember                      \
  FRMotor.MotorMember                      \
  FLMotor.MotorMember

#define Rads(Degs) ((3.1415926535/180.0)*Degs)
#define Degs(Rads) ((180.0/3.1415926535)*Rads)

#define MinVoltage 1.5

float GetClosestToZero(float First, float Second);
void Calibrate();
void Turn(float Turn_, float speed = 100, bool Wait_ = true, float CoustomTimeout_ = 2.0);
void TurnTo(float Turn_x, float Turn_y, float speed = 100, bool Wait_ = true, float Turn_Degree = 0, float Coustom_Timeout = 2.5);
void Drive(float Distance_, float Speed_V_ = 100, bool Wait_ = true, bool f_b = false, bool b_b = false, bool coast = false, float maxDistance = 10000000);
void DriveFast(float Distance_, float Speed_V_ = 100, bool Wait_ = true, bool f_b = false, bool b_b = false, bool coast = false, float maxDistance = 10000000);

void DriveTo(float Drive_x, float Drive_y, float speed, float radius = 0, bool Wait_ = true, float Coustom_Timeout = 5);
void StartTasks();
void TurnAndDrive(float x_point, float y_point, float driveSpeed = 80, float turnSpeed = 80, float radius = 0, bool faceDirection = true, bool driveWait = true, float TurnTimeout = 2.5, float DriveTimeout = 5);
