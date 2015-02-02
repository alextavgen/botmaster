//
// C++ Interface: suursilm
//
// Description: 
//
//
// Author: Valdur Kaldvee, Margus Ernits <margus.ernits@itcollege.ee>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef JPROBOT_H
#define JPROBOT_H
#include "../../abstractrobot.h"
#include "../../objecttype.h"
#include "../../comm.h"
#include <iostream>
#include "jptopsecretmasterclass.h"

/**
	@author Kristjan Molder, Margus Ernits <margus.ernits@itcollege.ee>
*/
class Jprobot: public AbstractRobot{
public:
	Jprobot();

	~Jprobot();

	void go();
	void clearCameraBuf();
	void msleep(long t) {usleep(t * 1000);}
	int round(float);
	void omni(float, float, int, float);
	void omniBreak();
	IplImage* getImg();
	Image* getImage();
	View* getView();

	StatClass ylemineV;
	StatClass ylemineP;
};

#endif
