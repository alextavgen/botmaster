/**
 * @file Palmer2.h
 * @brief C++ Interface: Palmer2
 *
 * @details Configuration module for Robot Vision System.
 *
 * Copyright: See COPYING file that comes with this distribution
 *
 * @author Valdur Kaldvee (C) 2008
 * @author Margus Ernits <margus.ernits@itcollege.ee>, (C) 2008
 * @author Mauno Pihelgas <mpihelga@itcollege.ee>, (C) 2010
 */
#ifndef Palmer2_H
#define Palmer2_H

#include "abstractrobot.h"
#include "image.h"
#include "view.h"
#include "comm.h"


/**
 * @brief Common class for testing robot functionality across all teams.
 * @author Valdur Kaldvee (C) 2008
 * @author Margus Ernits <margus.ernits@itcollege.ee>, (C) 2008
 * @author Mauno Pihelgas <mpihelga@itcollege.ee>, (C) 2010
 */
class Palmer2: public AbstractRobot{
public:



    Palmer2();

    ~Palmer2();
    void stateChange();
    bool gotBall ();
    bool isSolenoidFull();
    bool isKickWayFree ();
    bool isThereBall();
    int calculateSpeed();
    int calculateRotation();
    bool isMoveFree ();
    bool isAimed();
    bool isGoalOn();
    void processFrame();
    int getVillainGoal();
    int getOwnGoal();
    int getGoalColor();
    void grabBallInFront();
    void sleep (long);
    void go();
    void printState();
    void stop();
    void omni(float, float, int, float, float);
    void printImageInfo(Image::Object *obj);
    void searchBall();
    void kick();
    void searchGoal();

    void catchBall();
    void findKickWay();
    void avoidCollision ();
    void drive();
    void centerGoal();
    int ballSize;
    bool clearToShoot;
    int lastBallDirection;
    int shootAdjustDir;


    Image::Object ball;
    Image::Object goal;
    Image::Object ownGoal;

private:

};

#endif
