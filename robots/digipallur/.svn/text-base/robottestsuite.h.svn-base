//
// C++ Interface: testrobotsuite
//
// Description: 
//
//
// Author: Renno Reinurm (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ROBOT_TEST_SUITE_H
#define ROBOT_TEST_SUITE_H
#include "abstractrobot.h"
// #include "log.h"
// #include "objectfinder.h"
// #include "image.h"
// #include "view.h"


/**
	@author  Renno Reinurm
*/
class RobotTestSuite: public AbstractRobot{
public:
  
  RobotTestSuite();

  ~RobotTestSuite();
    
  virtual void go()=0;
  
  bool testAnalogInput(int analog_no);
  bool testDigitalInput(int digital_no);
  bool testDigitalOutput(int digital_no);
  void testMotor(int motor, int speed);
  void testServo(int servo, unsigned char speed);
  void testCamera();
  private:
   
};

#endif
