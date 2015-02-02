//
// C++ Implementation: robotfactory
//
// Description: 
//
//
// Author: Valdur Kaldvee, Margus Ernits <margus.ernits@itcollege.ee>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "robotfactory.h"
#include <string>
#include <iostream> //TODO remove after debug

using namespace std;

/*RobotFactory::RobotFactory()
{
}


RobotFactory::~RobotFactory()
{
}


*/

/*!
    \fn RobotFactory::getRobot(const * name)
 */
AbstractRobot  *RobotFactory::getRobot(const char * name)
{
    if (name!=NULL){
        string str_name(name);
        if(str_name=="digipallur"){
            return new DigiPallur;
        }else if(str_name=="digipallurtest"){
            return new DigiPallurTest;
        }else if(str_name=="neve"){
            return new Neve;
        }else if(str_name=="nevetest"){
            return new NeveTest;
        }else if(str_name=="tigetigu"){
            return new TestRobot;
        }else if(str_name=="TestRobot"){
            return new TestRobot;
        }else if(str_name=="jprobot"){
            return new Jprobot;
        }else if(str_name=="jpr"){
            return new Jpr;
        }else if(str_name=="jprtest"){
            return new JprTest;
        }else if(str_name=="tont"){
            return new Tont;
        }else if(str_name=="hal"){
            return new Hal;
        }else if(str_name=="haltest"){
            return new HalTest;
        }else if(str_name=="palmer"){
            return new Palmer;
        }else if(str_name=="palmer2"){
            return new Palmer2;
        }else if(str_name=="maiu"){
            return new Maiu;
        }else if(str_name=="maiutest"){
        return new MaiuTest;
        }

    }
      return new TestRobot;
}
