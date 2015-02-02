//
// C++ Implementation: Jprobot
//
// Description: 
//
//
// Author: Kristjan Molder (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "jprobot.h"
#include <QTime>
#include <QDebug>
#include <QCoreApplication>
#include "jpliikumisthread.h"
#include "jpimgprocthread.h"
#include "battery.h"



#define SOCK_VALUE 800


Jprobot::Jprobot()
{
std::cout <<"Jprobot ctor"<<std::endl;
}


Jprobot::~Jprobot()
{
}

void Jprobot::clearCameraBuf() {
	for (int i = 0; i < 5; i++) image->getFrame();
}

int Jprobot::round(float number)
{
    return (number >= 0) ? (int)(number + 0.5) : (int)(number - 0.5);
}

IplImage* Jprobot::getImg(){
	return _img;
}

Image* Jprobot::getImage(){
	return image;
}

View* Jprobot::getView(){
	return view;
}


void Jprobot::go()
{
	QTime t;
	

        qDebug("JP GO...\n");
	
	Config  & conf  = Config::getConfig();
	conf.setSendCmdEnabled(1);

	ImgProcThread imageProc(this);

	//imageProc.start();

	LiikumisThread liikumine(this, &imageProc);

	liikumine.start();

	//Battery battery(this);
	

	//std::cout << battery.getAsus() << "%" << std::endl;
	//std::cout << battery.getBattery() << std::endl;


	conf.setSendCmdEnabled(1);


	while(1){
		getSensors();

		if(!digital[7]){
			liikumine.maga = true;
		}else{
			liikumine.maga = false;
		}	
		//std::cout<<"lyliti: "<<digital[7]<<std::endl;

		liikumine.mainKickSolenoid();
		ylemineV.insert(analog[4]);
		ylemineP.insert(analog[5]);
		
		msleep(2);
  		
	}
	
}
