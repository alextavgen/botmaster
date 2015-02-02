#ifndef JPIMGPROCTHREAD_H
#define JPIMGPROCTHREAD_H
#include <QThread>
#include <iostream>
#include "../../abstractrobot.h"
#include "../../objecttype.h"
#include "../../comm.h"
#include "jprobot.h"
#include "jptopsecretmasterclass.h"
#include <QTime>

class ImgProcThread : public TopSecretMasterClass{
public:
	ImgProcThread(Jprobot*);
	Jprobot* jpr;
	IplImage* _img;
	Image::Object *obj;
	Image::Object *Ball;
	Image::Object *BallPrev;
	int BallFrame;
	int BallPrevFrame;
	int curFrame;
	QTime timer;
	int timeDif;
	double distX;
	double distY;
	double dist;
	double distYDif;
	double distXDif;
	double distDif;
	double nurkDif;
	double nurkDeg,nurkRad;
	double speed;
	double speedX;
	double speedY;
	double speedN;

	virtual void run();
	void imgProcess();
	void calculate();
	bool isValidBall(Image::Object*);
	void CopyBall();
	Image::Object* getBall();
	Image::Object* getBallPrev();
	bool updated;
	bool isUpdated();
	void printBall(Image::Object *B);
	//double speedY();
	//double speedX();
	//double speedN();
	double calcDist(double deg);
};





#endif
