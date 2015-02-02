//
// C++ Interface: robotfactory
//
// Description: 
//
//
// Author: Margus Ernits <margus.ernits@itcollege.ee>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ROBOTFACTORY_H
#define ROBOTFACTORY_H
#include "abstractrobot.h"
#include "robots/testrobot/testrobot.h"
#include "robots/digipallur/digipallur.h"
#include "robots/digipallur/digipallurtest.h"
#include "robots/pelmeen/pelmeen.h"
#include "robots/jprobot/jprobot.h"
#include "robots/jpr/jpr.h"
#include "robots/jpr/jprtest.h"
#include "robots/tont/tont.h"
#include "robots/hal/hal.h"
#include "robots/neve/neve.h"
#include "robots/neve/nevetest.h"
#include "robots/hal/haltest.h"
#include "robots/palmer/palmer.h"
#include "robots/palmer2/palmer2.h"
#include "robots/maiu/maiu.h"
#include "robots/maiu/maiutest.h"

/**
	@author Margus Ernits <margus.ernits@itcollege.ee>
*/
class RobotFactory{
public:
   // RobotFactory();

   // ~RobotFactory();
  static AbstractRobot *getRobot(const char *);

};

#endif
