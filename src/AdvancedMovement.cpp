#include <vex.h>
#include <AdvancedMovement.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <algorithm>

using namespace std;

bool isInstalled;
void AdvancedMovement::setup() // YOU NEED TO CALL THIS BEFORE USING!!!!!!
{
  Inertial.calibrate();
  waitUntil(!Inertial.isCalibrating());
  isInstalled = Inertial.installed();
  if(!isInstalled)
    Brain.Screen.print("Inertial (IMU) Sensor not Installed");
}
int minError(double target, double current){
    double b = max(target,current);
    double s = min(target,current);
    int diff = b - s;
    if (diff <= 180)
        return diff;
    else
       return 360-b + s;
}
int dirToSpin(int target, int currHeading){
  int diff = target - currHeading;
  if(diff < 0)
      diff += 360;
  if(diff > 180)
      return 1;
  else
      return -1;
}
void AdvancedMovement::turnPID(int angle)
{ 
  double kP = 0.1;
  double kI = 0;
  double kD = 0;
  int integral;
  int error = minError(angle, Inertial.heading(degrees));
  int prevError = 0;
  int power = 0;
  int derivative = 0;
  while (abs(error) > 0.5)
  {
    Brain.Screen.print(error);
    Brain.Screen.newLine();
        Brain.Screen.print(Inertial.heading(degrees));
    Brain.Screen.print("Error: %i", error);
    error = minError(angle, Inertial.heading(degrees));
    integral = integral + error;
    if (error == 0 || error > angle){
      integral = 0;
    }
    derivative = error - prevError;
    prevError = error;
    power = error * kP; //+ integral *  kI * + derivative *  kD;
    //LeftDriveSmart.spin(fwd, -1 * dirToSpin(angle, Inertial.heading(degrees)) * power, volt);
    //RightDriveSmart.spin(fwd,  dirToSpin(angle, Inertial.heading(degrees)) * power, volt);
    LeftDriveSmart.spin(fwd, power, rpm);
    RightDriveSmart.spin(fwd, -1*power, rpm);
    wait(15, msec);
  }
  Drivetrain.stop(brakeType::brake);

}
