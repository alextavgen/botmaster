//
// C++ Implementation: MaiuTest
//
// Description: 
//
//
// Author: Valdur Kaldvee (C) 2010
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "maiutest.h"
#include <QTime>
#include <QDebug>
#include <iostream>

using namespace std;

MaiuTest::MaiuTest()
{
}

MaiuTest::~MaiuTest()
{
}

void MaiuTest::motorTest() {
	int i;
	
	for (i = 0; i < 3; i++) {
		setDcMotor(i, 100);
		msleep(2000);
		setDcMotor(i, 355);
		msleep(2000);
		setDcMotor(i, 0);
	}
}

void MaiuTest::solenoidTest() {
	setDigital(0);
	msleep(20);
	clearDigital(0);
}

void MaiuTest::dribblerTest() {
	setDcMotor(3, 255);
	msleep(5000);
	setDcMotor(3, 0);
}

void MaiuTest::go() {
  	int selection = 0;
	char str[50];
	int i;
	int height;
	
	qDebug("MaiuTest GO...\n");
	
	Config  & conf  = Config::getConfig();
	
	while(1) {
		requestSensors();
	
		_img = image->getFrame();
		image->process(1,0,0,0);

		getSensorsResponse();
		
		selection = conf.keyS;
  
		switch(selection) {
			case 1:
				sprintf(str, "Motor test");
				if (conf.getSendCmdEnabled()) {
					motorTest();
					conf.setSendCmdEnabled(0);
				}
				break;
			case 2:
				sprintf(str, "Solenoid test");
				if (conf.getSendCmdEnabled()) {
					solenoidTest();
					conf.setSendCmdEnabled(0);
				}
				break;
			case 3:
				sprintf(str, "Dribbler test");
				if (conf.getSendCmdEnabled()) {
					dribblerTest();
					conf.setSendCmdEnabled(0);
				}
				break;
			default:
				sprintf(str, "No test selected");
				break;
		}		
	
		// Show selected test text
		cvPutText(_img, str, cvPoint(60, 20), &(image->font), CV_RGB(0,0,0));
		
		// Show analog values
		height = 80;
		
		for (i = 0; i < 8; i++) {
			sprintf(str, "A%d %d", i, analog[i]);
			cvPutText(_img, str, cvPoint(10, height), &(image->font), CV_RGB(0,0,0));
			height += 20;
		}
		
		// Show digital values
		height = 80;
		
		for (i = 0; i < 8; i++) {
			sprintf(str, "D%d %d", i, digital[i]);
			cvPutText(_img, str, cvPoint(250, height), &(image->font), CV_RGB(0,0,0));
			height += 20;
		}
		
		view->show(_img);
	}
}
