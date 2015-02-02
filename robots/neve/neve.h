//
// C++ Interface: Neve
//
// Description: 
//
//
// Author: Valdur Kaldvee, Margus Ernits <margus.ernits@itcollege.ee>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef Neve_H
#define Neve_H
#include "abstractrobot.h"
#include "objecttype.h"
#include "comm.h"
/**
	@author Valdur Kaldvee, Margus Ernits <margus.ernits@itcollege.ee>
*/
class Neve: public AbstractRobot{
public:
        Neve();

        ~Neve();

	void go();
	void msleep(long t) {usleep(t * 1000);}
	int round(float);
	void startDribbler();
	void stopDribbler();
	void kickBall();
	int goalColor();
	int centerOnGoal(Image::Object goal, int turnSpeed);
        void squareDriveTest();
};

#endif
