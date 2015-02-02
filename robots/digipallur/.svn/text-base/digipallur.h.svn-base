//
// C++ Interface: DigiPallur
//
// Description: 
//
//
// Author: Valdur Kaldvee, Margus Ernits <margus.ernits@itcollege.ee>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DigiPallur_H
#define DigiPallur_H
#include "abstractrobot.h"
#include "objecttype.h"
#include "comm.h"
/**
	@author Valdur Kaldvee, Margus Ernits <margus.ernits@itcollege.ee>
*/
class DigiPallur: public AbstractRobot{
public:
        DigiPallur();

        ~DigiPallur();

	void go();
	void clearCameraBuf();
	void msleep(long t) {usleep(t * 1000);}
	int round(float);
	void omni(float, float, float);
	void omniBreak();
	void startDribbler();
	void stopDribbler();
	void kickBall();
};

#endif
