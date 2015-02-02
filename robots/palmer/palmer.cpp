//
// C++ Implementation: tolmurull
//
// Description: 
//
//
// Author: Valdur Kaldvee, Margus Ernits <margus.ernits@itcollege.ee>, Katrin Kibbal (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "palmer.h"
#include <math.h>
#include <QTime>
#define BALL_SENSOR 5
#define UP_LEFT_SENSOR 1
#define UP_RIGHT_SENSOR 0
#define DOWN_LEFT_SENSOR 7
#define DOWN_RIGHT_SENSOR 6
#define DOWN_SENSOR_THRESHOLD 230
#define UP_SENSOR_THRESHOLD 130
#define WALL_DRIVE_DEGREE 70
#define AVOID_GOAL_TURN_SPEED 120
#define AVOID_GOAL_TURN_TIME 400
#define BALL_SENSOR_THRESHOLD 700
#define CLEAR_TO_SHOOT_THRESHOLD 30
#define SHOOT_ADJUST_DEG 80
#define VILLAIN_GOAL_SWITCH 4
#define LEFT_GOAL_SENSOR 4
#define RIGHT_GOAL_SENSOR 2
#define BALL 1
#define BLUE_GOAL 2
#define YELLOW_GOAL 3
#define MAX_SPEED 190
#define MAX_TURN_SPEED 40
#define STATE_SEARCH_BALL 0
#define STATE_AIM 1
#define STATE_KICK 2
#define STATE_CATCH_BALL 3
#define STATE_SEARCH_GOAL 4
#define STATE_GOING_FORWARD 5
#define STATE_CLEAR_GOAL 6
#define BALL_SEARCH_SPEED 60
#define GOAL_SEARCH_SPEED 60
#define GOAL_PIXEL_ERROR 0
#define MAIN_SWITCH_SENSOR 5
#define ROUND_LENGTH_SEC 90
#define BALL_SEARCH_LENGTH_SEC 4
using namespace std;


Palmer::Palmer()
{
}

Palmer::~Palmer()
{
    stop();
}

void Palmer::sleep(long t)
{
    usleep(t * 1000);
}


void Palmer::omni(float dirDeg, float velocityBody, float velocityAngular) {
    
    cout<<"sõidukiirus: "<<velocityBody<<", pööramiskiirus: "<<velocityAngular<<" deg: "<<dirDeg<<endl;
    
    float velocityX, velocityY;
    float velocityWheel[3];
    float max = 0.0;
    float scaleCoef;
    float dirRad;
    int pwm[3];
    int pwmSum = 0;
    int direction;
    int i;
    
    dirRad = dirDeg * 0.0174532925; // degrees to radians
    
    velocityX = velocityBody * cos(dirRad);
    velocityY = velocityBody * sin(dirRad);
    
    // 150, 30, 270 (60, 300, 180)
    velocityWheel[0] = -0.866025404 * velocityX + 0.5 * velocityY + velocityAngular;
    velocityWheel[1] = 0.866025404 * velocityX + 0.5 * velocityY + velocityAngular; 
    velocityWheel[2] = -velocityY + velocityAngular;
    
    // Voltage is not linearly proposional to power
    max = 0.0;
    for(i = 0; i < 3; i++) {
        if (abs(velocityWheel[i]) > max) max = abs(velocityWheel[i]);
    }
    
    if (max > 0) {
        for(i = 0; i < 3; i++) {
            direction = 1;
            if (velocityWheel[i] < 0) direction = -1;
            velocityWheel[i] = sqrt((max*max) * (abs(velocityWheel[i]) / max)) * direction;
        }
    }
    
    // Scaling
    max = 0.0;
    for(i = 0; i < 3; i++) {
        if (abs(velocityWheel[i]) > MAX_SPEED) max = abs(velocityWheel[i]);
    }
    
    if (max > MAX_SPEED) {
        scaleCoef = MAX_SPEED / max;
        for(i = 0; i < 3; i++) {
            velocityWheel[i] = velocityWheel[i] * scaleCoef;
        }
    }
    
    for(i = 0; i < 3; i++) {
        pwm[i] = round(velocityWheel[i]);
        pwmSum = pwmSum + pwm[i];
    }
    
    for(i = 0; i < 3; i++) {
        // If not all wheels are stopped break wheels with 0 speed
        if (pwm[i] == 0 && pwmSum != 0) {
            setDcMotor(i, 512);
        } else {
            setDcMotor(i, pwm[i]);
//             cout<<" pwm: "<<pwm[i]<<endl;
        }
    }
}

int Palmer::calculateSpeed() {    
    if(ballSize < 40) {
        return MAX_SPEED;
    }
    int spd = -0.5 * ballSize + 160;
    int adjustRatio = 0;
    if(ball.distanceH != 0) {
//         adjustRatio = (abs(ball.distanceH) * -0.142857  + 100);
        adjustRatio = (abs(ball.distanceH) * -0.2222222  + 100);
        int adjustedAdjustCoef = (100 - adjustRatio) * ((ballSize - 50)/1.5) / 100;
        adjustedAdjustCoef = adjustedAdjustCoef > 100 ? 100 : adjustedAdjustCoef;
        adjustedAdjustCoef = adjustedAdjustCoef < 0 ? 0 : adjustedAdjustCoef;
//         cout<<"SOmething: "<<(ballSize - 30)/1.5<<endl;
//         cout<<"adjustedAdjustCoef: "<<adjustedAdjustCoef<<endl;
        adjustRatio = 100 - adjustedAdjustCoef ;
    }
    if(adjustRatio >= 0 && adjustRatio <= 100) {
        spd = spd * (adjustRatio/100.0);
    }
    return spd > MAX_SPEED ? MAX_SPEED : spd;
}

int Palmer::calculateRotation() {
    int spd = 0;
    int distanceH = ball.distanceH;
    
    if(distanceH != 0) {
        if (ballSize < 170) {
//             spd = 1.0956699469 * pow(abs(distanceH), 0.6065465143);
            spd = 1.05334966 * pow(abs(distanceH), 0.6422576078);
            spd = spd > 40 ? 40 : spd;
        } else if (ballSize > 70) {
//             spd = 1.3973530171 * pow(abs(distanceH), 0.5808808105);
            spd = 1.1030219792 * pow(abs(distanceH), 0.6721912621);
        }
    }
    spd = spd * -(distanceH/abs(distanceH));
    spd = spd > MAX_TURN_SPEED ? MAX_TURN_SPEED : spd;
    spd = spd < -MAX_TURN_SPEED ? -MAX_TURN_SPEED : spd;
    return spd;
}

int Palmer::calculateGoalRotation() { //väravaotsimine
    int distanceH = goal.distanceH;
    int dir = 1;
    if(distanceH != 0) {
        dir = -(distanceH/abs(distanceH));
    }
    goalForwardSpeed = 0;
    if(distanceH !=0) {
        if(abs(distanceH) < 320) {
            goalForwardSpeed = 55;

            if(goal.rect.height < 30) {
                if(abs(distanceH) < 160) {
                    return (0.05 * abs(distanceH) + 5) * dir;
                }
            }

            return (0.05 * abs(distanceH) + 15) * dir;
        }
    }
    return GOAL_SEARCH_SPEED;
}

bool Palmer::isAimed() {
    
    int h = goal.rect.height;    
    int dH = goal.distanceH;
    
    if( (abs(dH) < 60 && h > 18) || (abs(dH) < 30) ) {
        return TRUE;
    }
    return FALSE;
}

void Palmer::printImageInfo(Image::Object *kala) {
    std::cout<<" DistanceH: "<<kala->distanceH;
    std::cout<<" Width: "<<kala->rect.width;
    std::cout<<" Height: "<<kala->rect.height;
    std::cout<<" Area: "<<kala->area;
    std::cout<<" BallSize: "<<ballSize;
    std::cout<<" X: "<<kala->rect.x;
    std::cout<<" Y: "<<kala->rect.y;
    std::cout<<std::endl;
}

// switch 0 - YELLOW, 1 - BLUE
int Palmer::getVillainGoal() {
    return digital[VILLAIN_GOAL_SWITCH] ? GOAL_YELLOW : GOAL_BLUE ;
}

int Palmer::getOwnGoal() {
    return digital[VILLAIN_GOAL_SWITCH] ? GOAL_BLUE : GOAL_YELLOW ;
}

void Palmer::printState() {
    switch(state) {
        case STATE_SEARCH_BALL: 
            cout<<"otsin palli"<<endl;
            break;
        case STATE_AIM: 
            cout<<"sihin väravasse"<<endl;
            break;
        case STATE_KICK: 
            cout<<"löön palli"<<endl;
            break;
        case STATE_CATCH_BALL: 
            cout<<"sõidan palli peale"<<endl;
            break;
        case STATE_SEARCH_GOAL: 
            cout<<"otsin väravat"<<endl;
            break;
        case STATE_GOING_FORWARD: 
            cout<<"püüan palli otse sõites kinni"<<endl;
            break;
        default:
            cout<<"default"<<state<<endl;
    }
}
void Palmer::printSpeeds() {
    
}

void Palmer::stop() {
    setDcMotor(0, 512);
    setDcMotor(1, 512);
    setDcMotor(2, 512);
}

void Palmer::kick() {
    std::cout<<"KICK"<<std::endl;
    setDcMotor(3, 255);
    sleep(30);
    setDcMotor(3, 0);
}

bool Palmer::hasBall() {
    return analog[BALL_SENSOR] > BALL_SENSOR_THRESHOLD;
}

void Palmer::calculateState() {
    if(!hasBall()) {
        if(ballSize > 0) {
            if(ballSize > 250 && abs(ball.distanceH) < 150) {
                searchTime = ballSearchTimer.elapsed();
                state = STATE_GOING_FORWARD;
                return;
            }
            searchTime = ballSearchTimer.elapsed();
            state = STATE_CATCH_BALL;
            return;
        }
        state = STATE_SEARCH_BALL;
        return;
    }
    if(hasBall()) {
        searchTime = ballSearchTimer.elapsed();
        if(isAimed()) {
            if(!clearToShoot) {
                state = STATE_CLEAR_GOAL;
                return;
            }
            state = STATE_KICK;
            return;
        }
        state = STATE_SEARCH_GOAL;
        return;
    }
}

void Palmer::searchBall() {
    
    // TODO: timers and stuff
    int forwardSpeed = 0;
    
    Image::Object smallerGoal;
    smallerGoal.area = 0;
    smallerGoal.distanceH = 0;
    
    if(goal.area > 0 && goal.rect.height < 40) {
        smallerGoal = goal;
    }
    
    else if(ownGoal.area > 0 && ownGoal.rect.height < 40) {
        smallerGoal = ownGoal;
    }
    
    if((ballSearchTimer.elapsed() - searchTime) > BALL_SEARCH_LENGTH_SEC * 1000 
        && (
            (goal.area > 0 && goal.rect.height < 40) 
            || 
            (ownGoal.area > 0 && ownGoal.rect.height < 40) 
        )
    )
    {
        searchTime = ballSearchTimer.elapsed() - BALL_SEARCH_LENGTH_SEC * 1000;
        
        int spd = 0;
        int distanceH = smallerGoal.distanceH;
        
        if(distanceH != 0) {
            if (ballSize < 170) {
                spd = 1.0956699469 * pow(abs(distanceH), 0.6065465143);
                spd = spd > 40 ? 40 : spd;
            } else if (ballSize > 170) {
                spd = 1.3973530171 * pow(abs(distanceH), 0.5808808105);
            }
        }
        spd = spd * -(distanceH/abs(distanceH));
        spd = spd > MAX_TURN_SPEED ? MAX_TURN_SPEED : spd;
        spd = spd < -MAX_TURN_SPEED ? -MAX_TURN_SPEED : spd;
        
        if((ballSearchTimer.elapsed() - searchTime) > BALL_SEARCH_LENGTH_SEC * 1000 * 2) {
            searchTime = ballSearchTimer.elapsed();
        }
        
        omni(0, 150, spd);
        return;
    }
    omni(0, 0, BALL_SEARCH_SPEED * lastBallDirection);
}

void Palmer::processSensorsData() {
//     goalSearchDir = digital[0];
    // TODO: timers and stuff
//     omni(0,0,BALL_SEARCH_SPEED * lastBallDirection);
}

void Palmer::searchGoal() {
    int rotation = calculateGoalRotation();
    int goalDir = (analog[LEFT_GOAL_SENSOR] > (analog[RIGHT_GOAL_SENSOR] + 200)) ? -1 : 1;
    omni(0, goalForwardSpeed, rotation);
}

void Palmer::clearGoal() {
    int deg = 0;
    if(leftSensor > DOWN_SENSOR_THRESHOLD) {
        deg = 90;
    }else if(rightSensor > DOWN_SENSOR_THRESHOLD) {
        deg = - 90;
    }else {
        deg = 90 * shootAdjustDir;
    }
    omni(deg, 50, 0);
}

void Palmer::grabBallInFront() {
    omni(0, 100, 0);
    sleep(500);
    processFrame();
}

void Palmer::chaseBall() {
    
    
    if(analog[UP_LEFT_SENSOR] < UP_SENSOR_THRESHOLD && analog[UP_RIGHT_SENSOR] < UP_SENSOR_THRESHOLD) {
        upSensorCounter = 0;
    }
    
    if(analog[UP_LEFT_SENSOR] > UP_SENSOR_THRESHOLD || analog[UP_RIGHT_SENSOR] > UP_SENSOR_THRESHOLD) {
        upSensorCounter++;
        if(upSensorCounter > 2) {
            lastBallDirection = analog[UP_LEFT_SENSOR] > analog[UP_RIGHT_SENSOR] ? -1 : 1;
            omni(0, 0, AVOID_GOAL_TURN_SPEED * lastBallDirection);
            sleep(AVOID_GOAL_TURN_TIME);
            return;
        }
    }
    int dirDeg = 0;
    int left = analog[DOWN_LEFT_SENSOR];
    int right = analog[DOWN_RIGHT_SENSOR];
    if(left > DOWN_SENSOR_THRESHOLD || right > DOWN_SENSOR_THRESHOLD) {
	if(left > DOWN_SENSOR_THRESHOLD && right > DOWN_SENSOR_THRESHOLD) {
	    omni(0, calculateSpeed(), calculateRotation());
	}
        int x = left > right ? left : right;
        int dir = left > right ? -1 : 1;
        int initialCoef = (0.75*ballSize - 27.5);
        int coef = 1;
        if(initialCoef < 10) {
            coef = 10;
        }
        else if(initialCoef > 100) {
            coef = 100;
        }
        else {
            coef = (0.75*ballSize - 27.5);
        } 
        dirDeg = (0.2*x + 40) * coef/100.0 * dir;
    }
    omni(dirDeg, calculateSpeed(), calculateRotation());
}

void Palmer::drive() {
    switch(state) {
        case STATE_CATCH_BALL:
            chaseBall();
            break;
        case STATE_SEARCH_BALL:
            searchBall();
            break;
        case STATE_SEARCH_GOAL:
            searchGoal();
            break;
    case STATE_CLEAR_GOAL:
            clearGoal();
            break;
    case STATE_GOING_FORWARD:
            grabBallInFront();
            break;
        case STATE_KICK:
            kick();
            break;
        default:
            break;
    }
    
}

void Palmer::processFrame() {
    
    _img = image->getFrame();
    image->process(0,0,0,0);
    
    ballSize = 0;
    
    ball.area = 0;
    ball.rect.y = 0;
    ball.rect.width = 0;
    ball.rect.height = 0;
    ball.distanceH = 0;
    goal.area = 0;
    goal.distanceH = 999;
    ownGoal.area = 0;
    ownGoal.distanceH = 999;
    
    clearToShoot = TRUE;
    
    Image::Object * obj;
    
    if (image->found_objects != NULL) {
        
        // Find biggest goal
        for (int i = 0; i < image->found_objects->total; i++) {
            obj = (Image::Object* ) cvGetSeqElem((image->found_objects), i);
            if (obj->type == getVillainGoal()) {
                if (obj->area > goal.area) { 
                    goal = *obj;
                }
            }
            else if (obj->type == getOwnGoal()) {
                if (obj->area > ownGoal.area) { 
                    ownGoal = *obj;
                }
            }
        }
        
        for (int i = 0; i < image->found_objects->total; i++) {
            obj = (Image::Object* ) cvGetSeqElem((image->found_objects), i);
            // Find closest ball    
            if (obj->type == BALL) {                    
                if (obj->rect.y > ball.rect.y) {
                    // TODO: ball that is 100% inside goal is wrong
                    if(
                        (goal.area != 0 &&
                            (
                                ((goal.rect.y + goal.rect.height) < (obj->rect.y + obj->rect.height)*1.05) 
                                    || 
                                (
                                    ((goal.rect.x + goal.rect.width) < (obj->rect.x + obj->rect.width))
                                    || 
                                    (goal.rect.x > obj->rect.x)
                                )
                            )
                        )
                        || goal.area == 0)
                    {
                        ball = *obj;
                    }
                    
                    if(ball.rect.height > 25 && ball.distanceH != 0) {
                        lastBallDirection = -ball.distanceH/abs(ball.distanceH);
                    }
                    
                    if((abs(ball.distanceH) - ball.rect.width ) < CLEAR_TO_SHOOT_THRESHOLD 
                        && ball.distanceH != 0) {
                        clearToShoot = FALSE;
                        shootAdjustDir = -ball.distanceH/abs(ball.distanceH);
                    }
                }
            }
        }
        ballSize = (ball.rect.width > ball.rect.height) ? ball.rect.width : ball.rect.height;
    }
    view->show(_img);
}

void Palmer::go() 
{
    Config  & conf  = Config::getConfig();
        
    QTime main;
    main.start();
    
    ballSearchTimer.start();
    
    
    qDebug("Palmer Start...\n");
    conf.setSendCmdEnabled(1);
    
    stop();
    
    lastBallDirection = 1;
    
    int stopped = 0;
    
    while(0) {
//         omni(80, 50, 0);
        cout<<ballSearchTimer.elapsed()<<endl;
        cout<<ballSearchTimer.elapsed()<<endl;
//         ballSearchTimer.start();
        
    }
    
    while(0) {
        
//         if(timer1.elapsed()/1000 > 90000) {
//             stop();
//             exit(0);
//         }
        
        requestSensors();
        
//         processFrame();
        
        getSensorsResponse();
        
        cout<<"a0: "<<analog[0]<<endl;
        cout<<"a1: "<<analog[1]<<endl;
        cout<<"a2: "<<analog[2]<<endl;
        cout<<"a3: "<<analog[3]<<endl;
        cout<<"a4: "<<analog[4]<<endl;
        cout<<"a5: "<<analog[5]<<endl;
        cout<<"a6: "<<analog[6]<<endl;
        cout<<"a7: "<<analog[7]<<endl;
        
        cout<<"d0: "<<digital[0]<<endl;
        cout<<"d1: "<<digital[1]<<endl;
        cout<<"d2: "<<digital[2]<<endl;
        cout<<"d3: "<<digital[3]<<endl;
        cout<<"d4: "<<digital[4]<<endl;
        cout<<"d5: "<<digital[5]<<endl;
        cout<<"d6: "<<digital[6]<<endl;
        cout<<"d7: "<<digital[7]<<endl;
//         
//         sleep(500);
        
//         omni(0,0, 40);
        
//         searchGoal();
//         
//         if(isAimed() && hasBall()) {
//             kick();
// //             sleep(1000);
//         }
        
    }
    
    while(1) {
        
        
        
        while(digital[MAIN_SWITCH_SENSOR] || stopped) {
            requestSensors();
            getSensorsResponse();
            if(digital[MAIN_SWITCH_SENSOR]) {
                stopped = 0;
            }
            stop();
            main.restart();
        }
        if(main.elapsed() > ROUND_LENGTH_SEC * 1000) {
            stop();
            stopped = 1;
            // all timers reset
            // all counters reset
            // wait for on switch
        }        
        
        requestSensors();
        
        processFrame();
        
        getSensorsResponse();
        
        processSensorsData();
        
        calculateState();
        
        printState();
        
        drive();
        
        printImageInfo(&goal);
//         printImageInfo(&ball);
        
//         if(analog[0] > UP_SENSOR_THRESHOLD || analog[1] > UP_SENSOR_THRESHOLD) {
//         cout<<"a0: "<<analog[0]<<endl;
//         cout<<"a1: "<<analog[1]<<endl;
//         }
    }
}
