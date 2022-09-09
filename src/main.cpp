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
class graph {


  #define NUM_POINTS  480
  
  private:
    // class to hold points for a single graph line
    class points {
      public:
        uint32_t         *_points;
        vex::brain::lcd  &_screen;
        vex::color        _color;
      
        points( vex::brain::lcd &screen ) : _screen(screen) {
          // allocate memory on heap
          _points = new uint32_t[NUM_POINTS];
          // init everything to some value we consider invalid
          for(int i=0;i<NUM_POINTS;i++) {
            _points[i] = INT32_MAX;
          }
          // default line color
          _color = vex::white;
        }
        ~points(){
          // deallocate memory
          delete _points;
        }

        // draw the line
        // There's a variety of ways to do this, could be another property of the class
        void draw() {
          _screen.setPenColor( _color );
          for(int x=0;x<NUM_POINTS-2;x++) {
            if( _points[x] != INT32_MAX ) {
              _screen.drawLine( x, _points[x], x+1, _points[x+1]);
              _screen.drawCircle( x, _points[x], 2, _color );
            }
          }
        }

        // add a point to this line
        void addPoint( int value ) {
          for(int i=0;i<NUM_POINTS-1;i++) {
            _points[i] = _points[i+1];
          }
          _points[NUM_POINTS-1 ] = value;
        }

        // set color for this line
        void setColor( vex::color c ) {
          _color = c;
        }
    };
    
    public:
      vex::brain _brain;
      std::vector<graph::points *> _points;
      int   _origin_x;
      int   _origin_y;
      
      graph( int seqnum, int origin_x, int origin_y ) : _origin_x(origin_x), _origin_y(origin_y) {
        // allocate and store each line
        for( int i=0;i<seqnum;i++ ) {
          _points.push_back( new graph::points(_brain.Screen) );
        }

        // thread to render this graph
        thread( render, static_cast<void *>(this) );
      }
      ~graph(){
        // we should deallocate the vector members here really
      }

      // Thread that constantly draws all lines
      static int render(void *arg ) {
        if( arg == NULL)
          return(0);

        graph *instance = static_cast<graph *>(arg);

        while( 1) {
            // this will call render, no need for any other delays
            instance->draw();
        }

        return(0);
      }

      // Draw graph X and Y axis
      // modify to fit your needs
      void drawAxis() {
        _brain.Screen.setPenColor( vex::white );
        _brain.Screen.drawLine( _origin_x, 0, _origin_x, 480 );
        _brain.Screen.drawLine( 0, _origin_y, 480, _origin_y );
        for( int x=0;x<480;x+=20 ) {
          _brain.Screen.drawLine( x, _origin_y+5, x, _origin_y-5 );
        }
        for( int y=0;y<240;y+=20 ) {
          _brain.Screen.drawLine( _origin_x+5, y, _origin_x-5, y );
        }
      }

      // draw everything
      void draw() {
        _brain.Screen.clearScreen( vex::color(0x202020) );
        drawAxis();
        for(int id=0;id<_points.size();id++)
          _points[id]->draw();
        _brain.Screen.render();
      }

      // add a point to a particular sequence
      void addPoint( int id, int value ) {
        if( id < _points.size() )
          _points[id]->addPoint(value + _origin_y );
      }
      
      // set the color of this sequence
      void setColor( int id, vex::color c ) {
        if( id < _points.size() )
          _points[id]->setColor( c );
      }
};
int MotorPower( void *arg ) {
    if( arg == NULL ) return 0;

    graph *g = static_cast<graph *>(arg);
    int y = 0; 
    int inc = 1;

    while(1) {

      g->addPoint( 3, -Controller.Axis3.position() + Controller.Axis1.position() );
      this_thread::sleep_for(5);
    }
}
void enableController()
{
      RemoteControlCodeEnabled = true;
}
void pre_auton(void) {
  
}



void autonomous(void) {
  
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
      Controller.Screen.print("Battery current: ");
      Controller.Screen.print(Brain.Battery.current());
  }
  return 1;
}
void usercontrol(void) {

  task controllerreadouts(controllerReadout);
  vexcodeInit();
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
    graph g( 4, 121, 240/2 );
    g.setColor(4, vex::color::green );

    // and we are using separate tasks to add points to each line, this is a bit overkill
    //thread t1( sinTask1, static_cast<void *>(&g) );
    //thread t2( sinTask2, static_cast<void *>(&g) );
    //thread t3( cosTask1, static_cast<void *>(&g) );
    //thread t4( triangleTask1, static_cast<void *>(&g) );
    thread t5 ( MotorPower, static_cast<void *>(&g) );
  
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
