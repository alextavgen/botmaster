//
// C++ Implementation: Neve
//
// Description: 
//
//
// Author: Valdur Kaldvee (C) 2012
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "neve.h"
#include <QTime>
#include <QDebug>
#include <math.h>
#include <iostream>

#define MAX_SPEED 255
#define ROUND_TIME 95

#define BEAM_BALL 600
#define BEAM_NO_BALL 400

#define WAITING_START 0
#define FIND_GOAL 1
#define	FIND_BALL 2
#define	GET_BALL 3
#define GOAL_RANDOM 4
#define BALL_RANDOM 5
#define NEW_KICK_POSITION 6

Neve::Neve() {
}

Neve::~Neve() {
}

int Neve::round(float number) {
    return (number >= 0) ? (int) (number + 0.5) : (int) (number - 0.5);
}

void Neve::squareDriveTest() {
    int speed = 50;
    int time = 2;

    setOmni(0, speed, 0);
    sleep(time);
    setOmni(90, speed, 0);
    sleep(time);
    setOmni(180, speed, 0);
    sleep(time);
    setOmni(270, speed, 0);
    sleep(time);
}

void Neve::startDribbler() {
    setDcMotor(3, 255);
}

void Neve::stopDribbler() {
    setDcMotor(3, 0);
}

void Neve::kickBall() {
    setDigital(0);
    msleep(20);
    clearDigital(0);
}

int Neve::goalColor() {
    if (digital[7] == 1) {
        return GOAL_RED;
    } else {
        return GOAL_BLUE;
    }
}

int Neve::centerOnGoal(Image::Object goal, int turnSpeed) {
    int goalLeft, goalRight, goalCenter;
    int center;
    int offset;

    offset = goal.rect.width / 20;

    goalLeft = goal.rect.x + offset;
    goalRight = goal.rect.x + goal.rect.width - offset;
    goalCenter = goalLeft + (goal.rect.width / 2);
    center = _img->width / 2;

    if (turnSpeed > 0) {
        if (center > goalLeft && center < goalCenter) return 1;
    } else {
        if (center < goalRight && center > goalCenter) return 1;
    }

    return 0;
}

void Neve::go() {
    Image::Object ball;
    Image::Object goal;
    Image::Object * obj;
    char str[50];

    int turnSpeed = 0;
    int forwardSpeed = 0;
    int angle = 0;
    int state = WAITING_START;
    int statePrev = 0;
    int nearBall = 0;
    int drivingForward = 0;
    int error = 0;
    int random = 0;
    int randomTurnDir = 0;
    int elapsed = 0;
    int goalSide = 0;
    int clearToKick = 0;
    int opponentGoal = 0;
    int selection = 0;
    int kickCount = 0;
    int frameCount = 0;
    int showFrameNr = 1;
    int ballTotal = 0;
    int ignoreBall = 0;
    float ballDistanceX = 0.0;  // cm
    float ballDistanceY = 0.0;  // cm
    float ballAngleDeg = 0.0;   // degrees
    float b = 0.0;
    int turnCoef = 0;
    int noBallTurnDir = 1;

    ball.distanceH = 0;

    QTime timer1;
    QTime timer2;
    QTime timer3;
    QTime timer4;
    QTime timer5;
    QTime timer7;
    QTime timer8;
    QTime timer9;
    QTime timer11;
    QTime timer12;
    QTime timer13;

    qDebug("Neve GO...\n");

    Config & conf = Config::getConfig();

    timer12.start();

    conf.setSendCmdEnabled(1);
    //while (1) squareDriveTest();

    while (1) {
        requestSensors();

        _img = image->getFrame();
        image->process(1, 0, 0, 0);

        getSensorsResponse();

        clearToKick = 1;
        if (image->found_objects != NULL) {
            ball.area = 0;
            ball.rect.y = 0;
            goal.area = 0;
            ballTotal = 0;

            // Find biggest goal
            for (int i = 0; i < image->found_objects->total; i++) {
                obj = (Image::Object*) cvGetSeqElem((image->found_objects), i);

                if (obj->type == opponentGoal) {
                    if (obj->area > goal.area) goal = *obj;
                }
            }

            // Find closest ball						
            for (int i = 0; i < image->found_objects->total; i++) {
                obj = (Image::Object*) cvGetSeqElem((image->found_objects), i);

                if (obj->type == BALL) {
                    if (obj->rect.y > ball.rect.y) ball = *obj;
                    if (abs(obj->distanceH) < 40) clearToKick = 0;
                    ballTotal++;
                }
            }
        }

        // Stop ignoring ball
        if (timer11.elapsed() > 350 || ballTotal != 1) ignoreBall = 0;

        if (ball.area != 0) {
            cvCircle(_img, ball.center, 3, CV_RGB(0, 0, 0), 5, CV_AA, 0);

            ball.center.y += conf.getTopCut(); // Adding ROI y min
            ballDistanceY = 1131458219.26 * pow((float) ball.center.y, -2.99);

            // b = tan(30 deg) * ballDistance
            b = 0.577350269 * ballDistanceY;

            ballDistanceX = (ball.distanceH / 320.0) * b;

            ballAngleDeg = atan(ballDistanceX / ballDistanceY) * 180 / M_PI;

            sprintf(str, "a: %.1f", ballAngleDeg);
            cvPutText(_img, str, cvPoint(ball.rect.x, ball.rect.y - 55), &(image->font), CV_RGB(0, 0, 0));

            sprintf(str, "x: %.0f", ballDistanceX);
            cvPutText(_img, str, cvPoint(ball.rect.x, ball.rect.y - 35), &(image->font), CV_RGB(0, 0, 0));

            sprintf(str, "y: %.0f", ballDistanceY);
            cvPutText(_img, str, cvPoint(ball.rect.x, ball.rect.y - 15), &(image->font), CV_RGB(0, 0, 0));
        }

        // show on image beacon reciver input
        //if (digital[4] == 0) cvCircle(_img, cvPoint(300, 20), 3, CV_RGB(255, 0, 0), 20, CV_AA, 0);
        //if (digital[5] == 0) cvCircle(_img, cvPoint(20, 20), 3, CV_RGB(255, 0, 0), 20, CV_AA, 0);

        // Robot gets start command
        if (state == WAITING_START && conf.getSendCmdEnabled()) {
            timer1.start();
            timer4.start();

            showFrameNr = 5;
            startDribbler();

            if (selection == 1) {
                setOmni(320, 200, 0);
                msleep(1500);
            }

            statePrev = state;
            state = FIND_BALL;
        }

        // Robot is waiting start
        if (state == WAITING_START) {
            //if (digital[6] == 0) conf.setSendCmdEnabled(1);		

            selection = conf.keyS;
            if (selection == 0) sprintf(str, "Normal");
            if (selection == 1) sprintf(str, "Diagonal");
            if (selection == 2) sprintf(str, "Vasak");
            if (selection == 3) sprintf(str, "Penalt");

            cvPutText(_img, str, cvPoint(10, 230), &(image->font), CV_RGB(0, 0, 0));

            opponentGoal = goalColor();

            // draw cirle above goal
            if (goal.area != 0) {
                cvCircle(_img, cvPoint(goal.rect.x + (goal.rect.width / 2), goal.rect.y - 14), 3, CV_RGB(0, 255, 0), 15, CV_AA, 0);
            }
        }

        // Robot is started
        if (state != WAITING_START) {
            elapsed = timer1.elapsed() / 1000;
            sprintf(str, "Time %d", elapsed);
            cvPutText(_img, str, cvPoint(10, 210), &(image->font), CV_RGB(0, 0, 0));

            // Time is up stopping the robot
            if (0 && elapsed >= ROUND_TIME) {   //TODO: put back
                setOmni(0, 0, 0);
                stopDribbler();
                conf.setSendCmdEnabled(0);
                selection = 0;
                kickCount = 0;
                showFrameNr = 1;
                state = WAITING_START;
            }
        }

        // Robot is searching the ball.
        if (state == FIND_BALL) {
            if (state != statePrev) {
                statePrev = state;
                nearBall = 0;
                randomTurnDir = 0;
                timer7.start();
                timer9.start();
                timer13.start();
            }

            if (analog[0] > BEAM_BALL) {
                setOmni(0, 0, 0);   // TODO: put back
                //state = GET_BALL;
            } else {
                if (0 && nearBall && (ball.area == 0 || ballDistanceY > 20.0)) {    //TODO: remove 0
                    state = GET_BALL;
                } else {
                    if (ball.area != 0 && !ignoreBall) {
                        timer9.start();

                        if (ballDistanceY < 10.0) nearBall = 1;

                        turnCoef = sqrt(35.0 - ballDistanceY);
                        if (turnCoef < 0) turnCoef = 0;

                        turnSpeed = sqrt(pow(45.0, 2.0) * (fabs(ballAngleDeg) / 30.0)) + turnCoef;
                        turnSpeed /= 3;
                        if (ballAngleDeg < 0) turnSpeed = -turnSpeed;

                        angle = 0;

                        if (selection == 3 && timer1.elapsed() < 700) {
                            forwardSpeed = 255;
                        } else {
                            forwardSpeed = 50;
                            /*
                            if (ballDistanceY < 200.0) forwardSpeed = 230;
                            if (ballDistanceY < 75.0) forwardSpeed = 170;
                            if (ballDistanceY < 30.0) forwardSpeed = 140;
                            if (ballDistanceY < 25.0) forwardSpeed = 130;
                            if (ballDistanceY < 20.0) forwardSpeed = 120;
                            */
                        }

                        setOmni(angle, forwardSpeed, turnSpeed);
                    } else {

                        if (0 && timer7.elapsed() > 15000) {     // TODO: remove 0
                            state = BALL_RANDOM;
                        } else {
                            forwardSpeed = 0;
                            turnSpeed = -20 * noBallTurnDir;
                            angle = 0;
                            setOmni(angle, forwardSpeed, turnSpeed);

                            /*
                            if (timer9.elapsed() > 10000) timer9.start();

                            if (timer9.elapsed() > 7000) {
                                angle = 0;

                                if (0) {   // TODO: black line analog[1] > 50 || analog[2] > 50
                                    forwardSpeed = 0;
                                    turnSpeed = -20;

                                    if (randomTurnDir == 0 && analog[1] > analog[2]) {
                                        randomTurnDir = 1;
                                    } else {
                                        randomTurnDir = 2;
                                    }

                                    if (randomTurnDir == 1) turnSpeed = -turnSpeed;
                                } else {
                                    forwardSpeed = 50;
                                    turnSpeed = 0;
                                    timer13.start();
                                }
                            } else {
                                forwardSpeed = 0;

                                if (timer13.elapsed() < 3000) {
                                    turnSpeed = -20 * noBallTurnDir;
                                } else {
                                    turnSpeed = -10 * noBallTurnDir;
                                }

                                angle = 0;
                            }
                            setOmni(angle, forwardSpeed, turnSpeed);
                            */
                        }
                    }
                }
            }
        }

        // Make solid contact with the ball.
        // Robot drives forward some time.
        if (state == GET_BALL) {
            if (state != statePrev) {
                statePrev = state;
                timer3.start();
            }

            turnSpeed = 0;
            forwardSpeed = 150;

            setOmni(0, forwardSpeed, turnSpeed);

            if (analog[0] > BEAM_BALL) {
                msleep(200);    // drive forward to make solid contact with ball
                state = FIND_GOAL;
            }

            if (timer3.elapsed() > 500) state = FIND_BALL;
        }

        // Remember if goal is right/left from robot according to beacon
        if (digital[4] == 0) goalSide = 1;
        if (digital[5] == 0) goalSide = 2;

        // Robot posesses ball and is searching the goal.
        // After finding the goal kicks ball in.
        if (state == FIND_GOAL) {
            if (state != statePrev) {
                statePrev = state;
                timer2.start();
            }

            if (analog[0] < BEAM_NO_BALL) {
                state = GET_BALL;
            } else {
                if (timer2.elapsed() > 6000) {
                    state = GOAL_RANDOM;
                } else {
                    // No goal on view
                    if (goal.area == 0) {
                        // Uses IR beacon to find goal
                        turnSpeed = -70;
                        if (goalSide == 1) turnSpeed = -turnSpeed;

                        angle = 0;
                        forwardSpeed = 0;

                        setOmni(angle, forwardSpeed, turnSpeed);
                    } else {
                        error = goal.distanceH;

                        if (centerOnGoal(goal, turnSpeed)) {
                            if (clearToKick) {
                                kickBall();
                                kickCount++;

                                std::cout << "Ball time: " << timer4.elapsed() / 1000 << std::endl;
                                timer4.start();
                                timer11.start();
                                ignoreBall = 1;

                                if (selection == 2 && kickCount == 1) {
                                    setOmni(0, 0, -200);
                                    msleep(300);
                                }

                                state = FIND_BALL;
                            } else {
                                state = NEW_KICK_POSITION;
                            }
                        } else {
                            forwardSpeed = 70;
                            angle = 0;

                            if (error == 0) {
                                turnSpeed = 0;
                            } else {
                                turnSpeed = log2f(abs(error));
                                turnSpeed *= 3;
                            }

                            if (error < 0) turnSpeed = -turnSpeed;
                            setOmni(angle, forwardSpeed, turnSpeed);
                        }
                    }
                }
            }
        }

        // Finds new kick position
        if (state == NEW_KICK_POSITION) {
            if (state != statePrev) {
                statePrev = state;
                timer8.start();
            }

            error = goal.distanceH;

            if (abs(error) < 1) {
                turnSpeed = 0;
            } else {
                turnSpeed = log2f(abs(error)) * 4.5;
                if (error < 0) turnSpeed = -turnSpeed;
            }

            turnSpeed = 0;
            angle = 60; // TODO: exists possiblility to drive over black line

            setOmni(angle, forwardSpeed, turnSpeed);

            if (timer8.elapsed() > 250) state = FIND_GOAL;
            if (analog[0] < BEAM_NO_BALL) state = GET_BALL;
        }

        // Random goal searching
        if (state == GOAL_RANDOM) {
            if (state != statePrev) {
                statePrev = state;
                drivingForward = 0;
                randomTurnDir = 0;
            }

            if (1 /*analog[1] > 100 || analog[2] > 100*/) {  //TODO: check where is black line
                forwardSpeed = 40;

                if (randomTurnDir == 0 && analog[1] > analog[2]) {
                    randomTurnDir = 1;
                } else {
                    randomTurnDir = 2;
                }

                turnSpeed = -80;
                if (randomTurnDir == 1) turnSpeed = -turnSpeed;
            } else {
                if (drivingForward == 0) {
                    timer5.start();
                    drivingForward = 1;
                    random = (rand() % 2000) + 500;
                }

                forwardSpeed = 150;
                turnSpeed = 0;

                if (timer5.elapsed() > random /*|| analog[1] > 150 || analog[2] > 150*/) {  // TODO: check black line
                    forwardSpeed = 0;
                    state = FIND_GOAL;
                }
            }

            setOmni(0, forwardSpeed, turnSpeed);
        }

        // Random ball searching
        if (state == BALL_RANDOM) {
            if (state != statePrev) {
                statePrev = state;
                drivingForward = 0;
                randomTurnDir = 0;
            }

            if (0 /*analog[1] > 50 || analog[2] > 50*/) {   // TODO: check black line
                forwardSpeed = 0;

                if (randomTurnDir == 0 /*&& analog[1] > analog[2]*/) {
                    randomTurnDir = 1;
                } else {
                    randomTurnDir = 2;
                }

                turnSpeed = -80;
                if (randomTurnDir == 1) turnSpeed = -turnSpeed;
            } else {
                if (drivingForward == 0) {
                    timer5.start();
                    drivingForward = 1;
                    random = (rand() % 4000) + 2000;
                }

                forwardSpeed = 150;
                turnSpeed = 0;

                if (timer5.elapsed() > random /*|| analog[1] > 150 || analog[2] > 150*/) { // TODO: check black line
                    forwardSpeed = 0;
                    state = FIND_BALL;
                }
            }

            setOmni(0, forwardSpeed, turnSpeed);
        }

        // Stateless actions
        if (state != WAITING_START) {
            // Avoid driving into goal

            // TODO: write new
            /*
            if (analog[3] > 300 && analog[1] <= 150 && analog[2] <= 150 && state != FIND_GOAL) {
                setOmni(0, 0, -100 * noBallTurnDir);
                msleep(200);
            }
            */
        }

        frameCount++;
        if (0  && frameCount >= showFrameNr) {
            view->show(_img);
            frameCount = 0;
        }

        // Reopen camera if fps low
//        if (image->fps < 10 && timer12.elapsed() > 2000) {
//            image->reOpen();
//            std::cout << "FPS is too slow...reopening Camera!!!" << std::endl;
//            timer12.start();
//        }
    }
}
