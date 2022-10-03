/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "buttonManager.h"
#include <iostream>
#include <algorithm>
#include <list>
#include <gifclass.h>
#include <vector>
#include "math.h"
#include "Vector2f.h"
#include <AdvancedMovement.h>

using namespace vex;


competition Competition;

button driverControlB; // Making the button.
button autonB;
button competitionB;
buttonManager bm;
// Struct square, x, y, width, height.
square drivercontrolD = { (480 - 121) / 5 - 50, 240 / 2 - 35, vexDisplayStringWidthGet("Enable Driver control."), vexDisplayStringHeightGet("Enable Driver control.")*4 };
square autonD = { (480 - 121) / 5 - 50, 10, vexDisplayStringWidthGet("Enable Autonomous.") + 15, vexDisplayStringHeightGet("Enable Autonomous.")*4 };
square competitionD = { (480 - 121) / 5 - 50, 240 / 2 + 45, vexDisplayStringWidthGet("Enable Competition Mode.") + 15, vexDisplayStringHeightGet("Enable Competition Mode.")*4 };
bool renderGif = true;
AdvancedMovement robotMovement;
Vector2f robotPosition;
short RobotRotation;


void turn(int x1, int y1, int x2, int y2)
{
  robotMovement.turnPID(atan2(y2 - y1, x2 - x1) * (180 / 3.14159265359));
  Brain.Screen.newLine();
  Brain.Screen.print((atan2(y2 - y1, x2 - x1) * (180 / 3.14159265359)));
}

void drive(int amount)
{
  Drivetrain.driveFor(fwd, amount, distanceUnits::in);
}
void enableController()
{
      RemoteControlCodeEnabled = true;
}
void pre_auton(void) {
  
}



void autonomous(void) {
  Drivetrain.setDriveVelocity(100, pct);
  Drivetrain.driveFor(fwd, 12, inches);
  turn(5, 5, 0, 1);

}

void SDcardInstalled() {
  Brain.Screen.setFont(fontType::mono12);
  Brain.Screen.setFillColor(transparent);
  if (Brain.SDcard.isInserted()) {
    Brain.Screen.setPenColor(green);
    Brain.Screen.printAt(250, 10, true, "SDcard: Installed!");
    Brain.Screen.setPenColor(white);
  } else {
    Brain.Screen.setPenColor(red);
    Brain.Screen.printAt(250, 10, true, "SDcard: Uninstalled!");
    Brain.Screen.setPenColor(white);
  }
  Brain.Screen.setFont(fontType::prop20);
}

void imuInstalled()
{
  Brain.Screen.setFont(fontType::mono12);
  Brain.Screen.setFillColor(transparent);
  if (Inertial.installed()) {
    Brain.Screen.setPenColor(green);
    Brain.Screen.printAt(250, 20, true, "IMU: Installed!");
    Brain.Screen.setPenColor(white);
  } else {
    Brain.Screen.setPenColor(red);
    Brain.Screen.printAt(250, 20, true, "IMU: Uninstalled!");
    Brain.Screen.setPenColor(white);
  }
  Brain.Screen.setFont(fontType::prop20);
}

int controllerReadout()
{
  while(true){
      Controller.Screen.setCursor(1,1);
      Controller.Screen.print("Throttle Left: ");

      Controller.Screen.print(Controller.Axis3.position() + Controller.Axis1.position());
      Controller.Screen.newLine();
      Controller.Screen.print("Throttle Right: ");
      Controller.Screen.print(Controller.Axis3.position() - Controller.Axis1.position());
      Controller.Screen.newLine();
      Controller.Screen.print("FlyWheel Speed: ");
      Controller.Screen.print(EncoderA.velocity(rpm));
  }
  return 1;
}

int invertDrive(directionType dir)
{

  bool dirB;
  (dir == fwd ? dirB = false : dirB = true);
  motor leftMotorA = motor(PORT1, ratio18_1, dirB);
  motor leftMotorB = motor(PORT2, ratio18_1, dirB);
  motor_group LeftDriveSmart = motor_group(leftMotorA, leftMotorB);
  motor rightMotorA = motor(PORT3, ratio18_1, dirB);
  motor rightMotorB = motor(PORT4, ratio18_1, dirB);
  motor_group RightDriveSmart = motor_group(rightMotorA, rightMotorB);
  drivetrain Drivetrain = drivetrain(LeftDriveSmart, RightDriveSmart, 319.19, 295, 40, mm, 1);
  return 0;
}
void trampoline()
{
    bool dirB;
    (LeftDriveSmart.direction() == fwd ? dirB = true : dirB = false);
    if(dirB == true){invertDrive(fwd);}
    else{invertDrive(directionType::rev);}
    
}
void usercontrol(void) {

  task controllerreadouts(controllerReadout);
  FlyWheel.spin(fwd, 100, velocityUnits::pct);
  vexcodeInit();
  Controller.ButtonL2.pressed(trampoline);
  
  

  while (true) {
    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

void addAllButtonsToManager(button buttons[])
{
  bm.addButtons(buttons, 3);
}

void driverControlButton()
{
  Brain.Screen.clearScreen();
  Brain.Screen.print("Driver control is being activated...");
  enableController();
  usercontrol();
}

void autonControlButton()
{
    Brain.Screen.clearScreen();
    Brain.Screen.print("Autonomous is being activated...");
    autonomous();
}


void competitionEnable()
{
  Brain.Screen.clearScreen();
  Brain.Screen.print("Competition Mode Enabled...");
  renderGif = false;

    
  
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  while(true)
  {
    wait(5, msec);
  }
}
int main() {

  Brain.Screen.drawLine(121, 0, 121, 240);
  Brain.Screen.setOrigin(121, 0);
  Brain.Screen.setFont(fontType::prop20);
  Brain.Screen.setPenWidth(2);

  Brain.Screen.setFillColor(white);
  Brain.Screen.setPenColor(black);
  driverControlB.setup(drivercontrolD, "Enable Driver control", driverControlButton); 
  autonB.setup(autonD, "Enable Autonomous.", autonControlButton);
  competitionB.setup(competitionD, "Enable Competition Mode.", competitionEnable);
  SDcardInstalled();
  imuInstalled(); 

  // Set up callbacks for autonomous and driver control periods.

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  //task autonCheck(checkCollision);
  button buttons[3] = 
  {
    driverControlB, autonB, competitionB
  };
  addAllButtonsToManager(buttons);
  Brain.Screen.setPenColor(white);


  vex::Gif gif("intro.gif", 0, 0 );
  
  //while(!renderGif) {
    //Brain.Screen.render();   
  //}// Changed heap size

}
