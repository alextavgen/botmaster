//
// C++ Interface: NeveTest
//
// Description: 
//
//
// Author: Valdur Kaldvee (C) 2010
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NEVE_TEST_H
#define NEVE_TEST_H
#include "robottestsuite.h"

class NeveTest: public RobotTestSuite
{
public:
	NeveTest();
	~NeveTest();
      	void go();
      	void msleep(long t) {usleep(t * 1000);}
	void motorTest();
	void solenoidTest();
	void dribblerTest();
	  
};
#endif
