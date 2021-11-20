void Calibrate();
void Turn(float Turn_, float speed = 100, bool Wait_ = true);
void TurnTo(float Turn_x, float Turn_y, float speed = 100, bool Wait_ = true, float Turn_Degree = 0, float Coustom_Timeout = 2.5);
void Drive(float Distance_, float Speed_V_ = 100, bool Wait_ = true, bool f_b = false, bool b_b = false, bool coast = false, float maxDistance = 10000000);
void DriveTo(float Drive_x, float Drive_y, float speed, float radius = 0, bool Wait_ = true, float Coustom_Timeout = 5);
void StartGps();
void TurnAndDrive(float x_point, float y_point, float driveSpeed = 80, float turnSpeed = 80, float radius = 0, bool faceDirection = true, bool driveWait = true, float TurnTimeout = 2.5, float DriveTimeout = 5);
