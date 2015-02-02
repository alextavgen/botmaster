//
// C++ Implementation: DigiPallur
//
// Description: 
//
//
// Author: Valdur Kaldvee (C) 2010
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "digipallur.h"
#include <QTime>
#include <QDebug>
#include <math.h>
#include <iostream>

#define MAX_SPEED 255
#define ROUND_TIME 95

#define BEAM_BALL 990
#define BEAM_NO_BALL 970

#define WAITING_START 0
#define FIND_GOAL 1
#define	FIND_BALL 2
#define	GET_BALL 3
#define GOAL_RANDOM 4
#define BALL_RANDOM 5
#define NEW_KICK_POSITION 6

DigiPallur::DigiPallur()
{
}


DigiPallur::~DigiPallur()
{
}

void DigiPallur::clearCameraBuf() {
	for (int i = 0; i < 5; i++) image->getFrame();
}

int DigiPallur::round(float number)
{
    return (number >= 0) ? (int)(number + 0.5) : (int)(number - 0.5);
}

void DigiPallur::omni(float dirDeg, float velocityBody, float velocityAngular) {
	float velocityX, velocityY;
	float velocityWheel[3];
	float max = 0.0;
	float scaleCoef;
	float dirRad;
	int pwm[3];
	int pwmSum = 0;
	int direction;
	int i;

	dirRad = dirDeg * 0.0174532925;	// degrees to radians

	velocityX = velocityBody * cos(dirRad);
	velocityY = velocityBody * sin(dirRad);

	//Vw[0] = round(cos(w1) * Vx + sin(w1) * Vy + L * Va);
	//Vw[1] = round(cos(w2) * Vx + sin(w2) * Vy + L * Va);	
	//Vw[2] = round(cos(w3) * Vx + sin(w3) * Vy + L * Va);

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
		}
	}
}

void DigiPallur::omniBreak() {
	setDcMotor(0, 512);
	setDcMotor(1, 512);
	setDcMotor(2, 512);
}

void DigiPallur::startDribbler() {
	setDcMotor(3, 511);
}

void DigiPallur::stopDribbler() {
	setDcMotor(3, 0);
}

void DigiPallur::kickBall() {
	setDcMotor(3, 255);
	msleep(20);
	setDcMotor(3, 0);
}

void DigiPallur::go()
{
	Image::Object ballMaxArea;
	Image::Object ballMaxAreaPrev;
	Image::Object goalMaxArea;
	Image::Object * obj;
	char str[10];
	
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
	int goalCenter = 0;
	int goalSide = 0;
	int goalSidePrev = 0;
	int clearToKick = 0;
	int opponentGoal = 0;
	int test1 = 0;
	int test2 = 0;
	int selection = 0;
	int kickCount = 0;
	
	float percent = 0.0;

	ballMaxArea.distanceH = 0;
	ballMaxAreaPrev.distanceH = 0;
	
	QTime timer1;
	QTime timer2;
	QTime timer3;
	QTime timer4;
	QTime timer5;
	QTime timer6;
	QTime timer7;
	QTime timer8;
	QTime timer9;
	QTime timer10;

	qDebug("DigiPallur GO...\n");
	
	Config  & conf  = Config::getConfig();

	while (0) {
		conf.setSendCmdEnabled(1);		
		getSensors();

		if (analog[5] > 400 || analog[6] > 400) { 
			if (analog[5] > 400 && analog[6] < 400) {
					turnSpeed = 70;
					forwardSpeed = 0;			
					angle = 0;

					omni(angle, forwardSpeed, turnSpeed);
			}

			if (analog[6] > 400 && analog[5] < 400) {
				turnSpeed = -70;
				forwardSpeed = 0;			
				angle = 0;

				omni(angle, forwardSpeed, turnSpeed); 
			}
		} else {
			turnSpeed = 0;
			forwardSpeed = 0;			
			angle = 0;

			omni(angle, forwardSpeed, turnSpeed); 
		}
	}

	while (0) {
		conf.setSendCmdEnabled(1);		
		getSensors();		

		if (digital[4] == 0) goalSide = 1;
		if (digital[5] == 0) goalSide = 2;
		if (digital[4] == 0 && digital[2] == 0) goalSide = 3;

		if (goalSide == 3) {
			omni(0, 0, 0);
		} else {
			if (goalSide == 1) {
				angle = 270;
				turnSpeed = 60;
			}
			
			if (goalSide == 2) {
				angle = 90;
				turnSpeed = -60;
			}					
						
			forwardSpeed = 100;
			omni(angle, forwardSpeed, turnSpeed);
		}
	}

	// Avoiding objects
	while (0) {
		conf.setSendCmdEnabled(1);		
		getSensors();

		if (analog[1] > 50) {
			turnSpeed = -100;
			omni(0, 150, turnSpeed);

			while (analog[1] > 50) {
				omni(0, 150, turnSpeed);
				getSensors();	
			}
		}

		if (analog[2] > 50) {
			turnSpeed = 100;
			omni(0, 150, turnSpeed);

			while (analog[2] > 50) {
				omni(0, 150, turnSpeed);				
				getSensors();	
			}
		}		
		
		if (analog[1] <= 70 && analog[2] <= 70) turnSpeed = 0;
		
		omni(0, 150, turnSpeed);
	}	
	
	// Square driving
	while(0) {
		conf.setSendCmdEnabled(1);		
		omni(0, 120, 0);
		msleep(2000);
		omni(90,120, 0);
		msleep(2000);
		omni(180, 120, 0);
		msleep(2000);
		omni(270, 120, 0);
		msleep(2000);
	}

	while(0) {
		conf.setSendCmdEnabled(1);		
		omni(0, 0, 120);
	}
	
	while(1){
		_img = image->getFrame();
		image->process(1,0,0,0);

		getSensors();

		clearToKick = 1;
		if (image->found_objects != NULL) {
			ballMaxArea.area = 0;
			goalMaxArea.area = 0;		
				
			for (int i = 0; i < image->found_objects->total; i++) {
				obj = (Image::Object* ) cvGetSeqElem((image->found_objects), i);
			
				if (obj->type == BALL) {
					percent = (obj->area * 100.0) / ballMaxArea.area;
	 			
					if (abs(obj->distanceH) < abs(ballMaxArea.distanceH)) {
						if (percent > 70.0) {
							ballMaxArea = *obj;
							ballMaxAreaPrev = ballMaxArea;
						}
					} else {
						if (percent > 130.0) {
							ballMaxArea = *obj;
							ballMaxAreaPrev = ballMaxArea;
						}
					}
		
					if (abs(obj->distanceH) < 20) clearToKick = 0;
				}

				if (obj->type == opponentGoal) {
					if (obj->area > goalMaxArea.area) {
						goalMaxArea = *obj;
					}
				}
		   	}
		}
		
		// Robot gets start command
		if (state == WAITING_START && conf.getSendCmdEnabled()) {			
			timer1.start();
			timer4.start();

			startDribbler();
			
			if (selection == 1) {
				omni(320, 200, 0);
				msleep(1500);
			}

			if (selection == 2) {
				omni(0, 255, 0);
				msleep(1000);
			}

			if (selection == 4) {
				forwardSpeed = 255;							
				
				while(timer4.elapsed() < 4000) {
					getSensors();
					turnSpeed = 0;
					if (digital[4] == 0 && digital[5] == 1) turnSpeed = 30;
					if (digital[5] == 0 && digital[4] == 1) turnSpeed = -30;

					omni(0, 255, turnSpeed);
				}
			}

			statePrev = state;
			state = FIND_BALL;
		}

		// show on image beacon reciver input
		if (digital[4] == 0) cvCircle(_img, cvPoint(300, 20), 3, CV_RGB(255,0,0), 20, CV_AA, 0);
		if (digital[5] == 0) cvCircle(_img, cvPoint(20, 20), 3, CV_RGB(255,0,0), 20, CV_AA, 0);
		
		// Robot is waiting start
		if (state == WAITING_START) {
			selection = conf.keyS;
			if (selection == 0) sprintf(str, "Normal");			
			if (selection == 1) sprintf(str, "Diagonal");
			if (selection == 2) sprintf(str, "Penalt");
			if (selection == 3) sprintf(str, "Vasak");
			if (selection == 4) sprintf(str, "Bermuda");
			
			cvPutText(_img, str, cvPoint(10, 230), &(image->font), CV_RGB(0,0,0));
		
			conf.setSendCmdEnabled(1);			
			if (digital[6] == 0) {
				opponentGoal = GOAL_RED;
			} else {
				opponentGoal = GOAL_BLUE;
			}
			conf.setSendCmdEnabled(0);
		}
		
		// Robot is started
		if (state != WAITING_START) {		
			elapsed = timer1.elapsed() / 1000;			
			sprintf(str, "Time %d", elapsed);
			cvPutText(_img, str, cvPoint(10, 210), &(image->font), CV_RGB(0,0,0));

			// Time is up stopping the robot
			if (elapsed >= ROUND_TIME) {
				omni(0, 0, 0);
				stopDribbler();
				conf.setSendCmdEnabled(0);
				selection = 0;
				kickCount = 0;
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
			}
			
			if (analog[0] > BEAM_BALL) {
				state = FIND_GOAL;
			} else {
				if (nearBall && (ballMaxArea.area == 0 || ballMaxArea.area < 1000)) {
					state = GET_BALL;
				} else {				
					if (ballMaxArea.area != 0) {					
						timer9.start();						

						if (ballMaxArea.area > 1500) nearBall = 1;					
						
						error = ballMaxArea.distanceH;
						
						if (abs(error) < 10) {
							turnSpeed = 0;
						} else {
							turnSpeed = log2f(abs(error)) * 7;						
							if (error < 0) turnSpeed = -turnSpeed;
						}

						angle = 0;
						forwardSpeed = 255;
											
						// Avoid hard collision with wall
						if (analog[1] > 100 || analog[2] > 100) forwardSpeed = 100;
						
						if (test2 == 1 || analog[1] > 250 && analog[2] < 200) {
							angle = 295;							
							test2 = 1;							
							turnSpeed = turnSpeed * 1.75;
						}
						
						if (test1 == 1 || (analog[2] > 250 && analog[1] < 200)) {
							angle = 65;
							test1 = 1;
							turnSpeed = turnSpeed * 1.75;
						}

						if ((test1 == 1 || test2 == 1) && abs(analog[2] - analog[1]) < 35) {
							test1 = 0;
							test2 = 0;
						}						

						// Limit turning at full speed
						if (forwardSpeed == 255) {						
							if (turnSpeed > 50) turnSpeed = 50;
							if (turnSpeed < -50) turnSpeed = -50;
						}

						omni(angle, forwardSpeed, turnSpeed);							
					} else {						
						if (timer7.elapsed() > 15000) {				
							state = BALL_RANDOM;
						} else {			
							if (timer9.elapsed() > 8000) timer9.start();

							if (timer9.elapsed() > 5000) {
								angle = 0;

								if (analog[1] > 50 || analog[2] > 50) {
									forwardSpeed = 0;
									turnSpeed = 110;

									if (randomTurnDir == 0 && analog[1] > analog[2]) {
										randomTurnDir = 1;
									} else {
										randomTurnDir = 2;
									}
							
									if (randomTurnDir == 1) turnSpeed = -1 * turnSpeed;
								} else {
									forwardSpeed = 255;								
									turnSpeed = 0;		
								}
							} else {
								forwardSpeed = 0;								
								turnSpeed = 140;
								angle = 0;
							}
							omni(angle, forwardSpeed, turnSpeed);
						}
					}
				}
			}
		}

		// Robot is so close to the ball that ball has moved under camera view.
		// Robot drives forward some time.
		if (state == GET_BALL) {
			if (state != statePrev) {
				statePrev = state;
				timer3.start();
			}
			
			turnSpeed = 0;
			forwardSpeed = 255;

			// Avoid hard collision with wall
			if (analog[1] > 100 || analog[2] > 100) forwardSpeed = 100;

			omni(0, forwardSpeed, turnSpeed);

			if (analog[0] > BEAM_BALL) {
				msleep(150);
				//clearCameraBuf();			
				state = FIND_BALL;
			}

			if (timer3.elapsed() > 500) state = FIND_BALL;
		}
		
		// Robot posesses ball and is searching the goal.
		// After finding the goal kicks ball in.
		if (0 && state == FIND_GOAL) {
			if (state != statePrev) {
				statePrev = state;
				goalSide = 0;
				goalSidePrev = 0;
				goalCenter = 0;
				timer2.start();
			}
			
			if (analog[0] < BEAM_NO_BALL) {	
				state = GET_BALL;
			} else {
				if (timer2.elapsed() > 6000) {	
					state = GOAL_RANDOM;
				} else {
					if (digital[4] == 0) goalSide = 1;
					if (digital[5] == 0) goalSide = 2;
					if (digital[4] == 0 && digital[5] == 0) goalSide = 3;
					
					// Kick ball if both sensors see goal or seeing flips sensors
					if (goalSide == 3 || (goalSide == 1 && goalSidePrev == 2) || (goalSide == 2 && goalSidePrev == 1)) {
						if (goalCenter == 1) {
							if (clearToKick == 1) {
								kickBall();
								kickCount++;

								//clearCameraBuf();
								std::cout<<"Ball time: "<<timer4.elapsed() / 1000<<std::endl;
								
								if (selection == 3 && kickCount == 1) {
									omni(0, 0, -200);
									msleep(300);	
								}

								timer4.start();
							
								state = FIND_BALL;
							} else {
								state = NEW_KICK_POSITION;	
							}
						} else {
							goalCenter = 1;
						}					
					} else {
						forwardSpeed = 0;
					
						// No goal on view
						if (digital[4] == 1 && digital[5] == 1) {
							turnSpeed = 110;
							timer10.start();
						} else {					
							if (turnSpeed > 80) turnSpeed = 0;							

							if (timer10.elapsed() < 100 && turnSpeed == 0) {
								turnSpeed = 0;
							} else {
								turnSpeed = 80;
							}
						}	

						if (goalCenter == 1) turnSpeed = 60;
						if (goalSide == 2) turnSpeed = -turnSpeed;

						omni(0, forwardSpeed, turnSpeed);
					}
					goalSidePrev = goalSide;
				}
			}
		}
				
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
					if (goalMaxArea.area == 0) {
						forwardSpeed = 0;
						turnSpeed = 110;
						omni(0, forwardSpeed, turnSpeed);
					} else {
						error = goalMaxArea.distanceH;
					
						// Kick ball if goal in center
						if (abs(error) < 6) {
							if (clearToKick == 1) {
								kickBall();
								kickCount++;

								//clearCameraBuf();
								std::cout<<"Ball time: "<<timer4.elapsed() / 1000<<std::endl;
								
								if (selection == 3 && kickCount == 1) {
									omni(0, 0, -200);
									msleep(300);	
								}

								timer4.start();
								state = FIND_BALL;
							} else {
								state = NEW_KICK_POSITION;	
							}
						} else {
							forwardSpeed = 100;
							turnSpeed = log2f(abs(error)) * 7;
							if (error < 0) turnSpeed = -turnSpeed;
							omni(0, forwardSpeed, turnSpeed);
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

			turnSpeed = 0;
			forwardSpeed = 160;
			angle = 60;
			omni(angle, forwardSpeed, turnSpeed);

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
			
			if (analog[1] > 100 || analog[2] > 100) {
				forwardSpeed = 40;				
				
				if (randomTurnDir == 0 && analog[1] > analog[2]) {
					randomTurnDir = 1;
				} else {
					randomTurnDir = 2;
				}
				
				turnSpeed = 80;			
				if (randomTurnDir == 1) turnSpeed = -turnSpeed;
			} else {
				if (drivingForward == 0) {
					timer5.start();
					drivingForward = 1;
					random = (rand()%2000)+500;
				}				

				forwardSpeed = 255;	
				turnSpeed = 0;

				if (timer5.elapsed() > random || analog[1] > 150 || analog[2] > 150) {
					forwardSpeed = 0;
					state = FIND_GOAL;
				}								
			}

			omni(0, forwardSpeed, turnSpeed);
		}

		// Random ball searching
		if (state == BALL_RANDOM) {
			if (state != statePrev) {
				statePrev = state;
				drivingForward = 0;
				randomTurnDir = 0;
			}

			if (analog[1] > 50 || analog[2] > 50) {
				forwardSpeed = 0;

				if (randomTurnDir == 0 && analog[1] > analog[2]) {
					randomTurnDir = 1;
				} else {
					randomTurnDir = 2;
				}
				
				turnSpeed = 80;			
				if (randomTurnDir == 1) turnSpeed = -1 * turnSpeed;
			} else {
				if (drivingForward == 0) {
					timer5.start();
					drivingForward = 1;
					random = (rand()%4000)+2000;
				}				

				forwardSpeed = 255;	
				turnSpeed = 0;

				if (timer5.elapsed() > random || analog[1] > 150 || analog[2] > 150) {
					forwardSpeed = 0;
					state = FIND_BALL;
				}								
			}

			omni(0, forwardSpeed, turnSpeed);
		}
		
		// Stateless actions
		if (state != WAITING_START) {
			// Detection and actions if robot is stuck in corner or wall.
			if (analog[1] > 300 && analog[2] > 300) {
				if (timer6.elapsed() > 500) {			
					omni(180, 150, 0);
					msleep(1000);
					omni(0, 80, 150);
					msleep(200);
					clearCameraBuf();
				}
			} else {
				timer6.start();			
			}

			// Avoid driving into goal
			if (analog[3] > 300 && analog[5] <= 150 && analog[6] <= 150 && state != FIND_GOAL) {
				omni(0, 0, 140);
				msleep(400);
			}
		}
		
		sprintf(str, "A1 %d", analog[1]);
		cvPutText(_img, str, cvPoint(10, 150), &(image->font), CV_RGB(0,0,0));

		sprintf(str, "A2 %d", analog[2]);
		cvPutText(_img, str, cvPoint(10, 170), &(image->font), CV_RGB(0,0,0));
		
		view->show(_img);
	}
}
