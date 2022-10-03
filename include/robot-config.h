using namespace vex;

extern brain Brain;
extern controller Controller;
extern motor_group LeftDriveSmart;
extern motor_group RightDriveSmart;
extern drivetrain Drivetrain;
extern bool RemoteControlCodeEnabled;
extern inertial Inertial;
extern encoder EncoderA;
extern motor FlyWheel;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void);
