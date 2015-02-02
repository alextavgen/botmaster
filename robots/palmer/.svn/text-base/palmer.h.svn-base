//
// C++ Interface: palmer
//
// Description: 
//
//
// Author: Valdur Kaldvee, Margus Ernits <margus.ernits@itcollege.ee>, Katrin Kibbal (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PALMER_H
#define PALMER_H

#include "../../abstractrobot.h"
#include "image.h"
#include "view.h"
#include <iostream>

/**
 *	@author  Margus Ernits <margus.ernits@itcollege.ee>
 */



class Palmer: public AbstractRobot{
public:
    
    Palmer();
    
    ~Palmer();
    
    void go();
    
    // functions
    
    void sleep(long t);
    void omni(float dirDeg, float velocityBody, float velocityAngular);
    void stop();
    int calculateSpeed();
    int calculateRotation();
    int calculateGoalRotation();
    int getVillainGoal();
    int getOwnGoal();
    bool isAimed();
    bool hasBall();
    bool isValidBall();
    void kick();
    void drive();
    void printState();
    void printImageInfo(Image::Object *obj);
    void printSpeeds();
    void processFrame();
    void calculateState();
    void searchBall();
    void searchGoal();
    void clearGoal();
    void grabBallInFront();
    void chaseBall();
    void processSensorsData();
    
    
    // variables
    
    int state;
    
    int ballSize;
    
    int goalDirection;
    
    int lastBallDirection;
    
    bool clearToShoot;
    
    int goalSearchDir;
    int goalForwardSpeed;
    int shootAdjustDir;
    
    int searchTime;
    
    int leftSensor;
    int rightSensor;
    
    int aimDelay;
    
    int upSensorCounter;
    
    Image::Object ball;
    Image::Object goal;
    Image::Object ownGoal;
    
    
    QTime ballSearchTimer;
};

#endif
