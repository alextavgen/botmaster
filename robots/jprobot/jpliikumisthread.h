#ifndef JPLIIKUMISTHREAD_H
#define JPLIIKUMISTHREAD_H
#include <QThread>
#include <iostream>
#include "../../abstractrobot.h"
#include "../../objecttype.h"
#include "../../comm.h"
#include "jprobot.h"
#include "jpomniwheel.h"
#include "battery.h"
#include <QTime>
#include <QStack>
#include "jpimgprocthread.h"
#include "jptopsecretmasterclass.h"


/**
	@author 
*/

enum ESearchStatus {ss_avoidWall, ss_avoidGoal, ss_avoidRobot, ss_searchCircle, ss_quickTurn, ss_forward};
enum ESearchType {st_ball, st_goal, st_ignore};
enum ESearchDir {sd_minus, sd_plus, sd_ball, sd_goal, sd_ylemine, sd_wall};

class Pall{
	public:		
		int id;	// palli ID		
		Image::Object Ball;
		int frame;
		double distX;
		double distY;
		double distYDif;
		double distXDif;
		double xnurkDif;
		double nurkDeg,nurkRad;
	};

class LiikumisThread : public QThread {

public:	
	LiikumisThread(Jprobot*);
	LiikumisThread(Jprobot*,ImgProcThread*);
	void init(Jprobot*,ImgProcThread*);
	ImgProcThread* imgProc;
	virtual void run();

	void soidaPallile();
	void soidaPallile2();
	void soidaPallile3();
	void soidaPallile4();
	void soidaPallile5();
	void soidaPallile6();
	void solenoid();
	void DoSomething();

	void keeraVaravale();
	void LiikumisTEST0();
	void LiikumisTEST1();
	void LiikumisTEST2();
	void LiikumisTESTYmberPalli();
	void AlmostCompetitionCode0();
	void PooramisTEST();	
	
	void keeraPalli(int suund);
	void sihiNurka(int suund);

	void varavaSuund();
	void ylemineSuund();
	void palliSuund();
	void seinaSuund();

	void pallNurgas();
	//bool solLaeb;

	int kickFrame;
	int kickMs;

	Jprobot* jpr;
	COmni* rattad;
	int aku;

	//IplImage* _img;
	//Image::Object *obj;
	//Image::Object *Ball;
	//Image::Object *BallPrev;		// teha hoopis stack 5 elemendiga vms
	Image::Object* getBall();
	Image::Object* getBallPrev();
	//Image::Object BallNext;
	int getBallFrame(){return imgProc->BallFrame;};
	int getBallPrevFrame(){return imgProc->BallPrevFrame;};
	
	//double xspeed;
	//double xnurk;
	//QTime timer;
	QTime timer2;
	QTime masterTimer;
	/*int timeDif;
	double distX;
	double distY;
	double dist;
	double distYDif;
	double distXDif;
	double xnurkDif;*/
	double getTimeDif(){return imgProc->timeDif;};
	double getDistX(){return imgProc->distX;};
	double getDistY(){return imgProc->distY;};
	double getDist(){return imgProc->dist;};
	//double getNurk(){return imgProc->distX;};
	double getDistXDif(){return imgProc->distXDif;};
	double getDistYDif(){return imgProc->distYDif;};
	double getNurkDif(){return imgProc->nurkDif;};
	double getSpeedX(){return imgProc->speedX;};
	double getSpeedY(){return imgProc->speedY;};
	double getSpeed(){return imgProc->speed;};
	double getSpeedN(){return imgProc->speedN;};
	double getNurkRad(){return imgProc->nurkRad;};
	double getNurkDeg(){return imgProc->nurkDeg;};

	
	//double speedY();	//kiirus palli suhtes cm/s (y komponent)
	//double speedX(); //kiirus palli suhtes cm/s (x komponent)
	//double speedN();
	//double calcNurk();	// nurk kauguste järgi
	//double nurkDeg,nurkRad;
	//QStack<Pall> BallStack;

	int getCurFrame();
	int getCurMs();

	int getUpdated();
	int v_suund;
	int y_suund;
	int p_suund;
	int s_suund;
	//bool kadus;

	//void Hark();		// bulshit
	//double calcDist(double deg);

	//void seinaPall();
	void keeraRingi(int suund);

	//void calculate();

	void katsetused();

public:
	bool maga;
private:
	QMutex mutex;
	//void printBall(Image::Object *B);
	void printCalcs();

	int searchM;

	void search2();
	ESearchStatus searchStat;
	ESearchType searchTyp;
	ESearchDir searchDir;
	int searchFrame;
	int searchMs;
	void setSearch2(ESearchStatus stat, ESearchType typ);
	void setSearchStatus(ESearchStatus stat);
	void setSearchType(ESearchType typ);
	void setSearchDir(ESearchDir dir);

	void headLightsOn();
	void headLightsOff();
	void ymberPalli();

	bool anduritegaVarav();
	bool anduritegaRobot();
	int ylemineV;
	int ylemineP;
	int getYlemineV(){return ylemineV;};
	int getYlemineP(){return ylemineP;};
	
	bool isSolenoidReady();
public:
	void mainKickSolenoid();
};

#endif
