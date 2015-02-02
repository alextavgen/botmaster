#ifndef JPOMNIWHEEL_H
#define JPOMNIWHEEL_H
#include "../../abstractrobot.h"
#include "../../objecttype.h"
#include "../../comm.h"
#include "jprobot.h"

/**
	@author Kristjan Molder, Margus Ernits <margus.ernits@itcollege.ee>
*/
class COmni {
public:
	COmni(Jprobot*);	
	void go(){};
	void omni(float, float, int, float);
	void omniBreak();

	Jprobot* jpr;
};

#endif
