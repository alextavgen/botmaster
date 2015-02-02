#include "jpliikumisthread.h"
#include "../../image.h"
#include "../../comm.h"
#include <cmath>


void LiikumisThread::init(Jprobot* v,ImgProcThread* im){
	jpr = v;
	imgProc = im;
	rattad = new COmni(jpr);

	v_suund = 1;
	y_suund = 1;
	p_suund = 1;
	s_suund = 1;
	//BallStack = new QStack<Pall*>();
	//kadus = false;
	timer2.start();
	masterTimer.start();
	maga=true;
	kickFrame = 0;
	kickMs = 0;
	
	searchStat = ss_searchCircle;
	searchTyp = st_ball;
	searchDir = sd_ball;
	searchFrame = 0;
	searchMs = 0;

	aku = 0;

	ylemineV = 0;
	ylemineP = 0;
}

LiikumisThread::LiikumisThread(Jprobot* v,ImgProcThread* im){	
	init(v,im);
}

LiikumisThread::LiikumisThread(Jprobot* v){
	init(v,NULL);
}

void LiikumisThread::run()
{

	//Ball = NULL;
	//BallPrev = 0;
	

	//rattad -> omni(0, 100, 512, 0);

	//_img = jpr->getImg();
	int c=0;

	jpr->getSensors();

	aku = jpr->analog[4];

	katsetused();
	
	int sek=0;
	//timer.start();

	while(1){ 		
		
		if(false){
			rattad->omniBreak();
			sek=2;
		}else{
			if(sek>=1){			
				sleep(sek);			
				sek=0;
			}
		//jpr->getSensors();
		ylemineP = jpr->ylemineP.avg();
		ylemineV = jpr->ylemineV.avg();
		aku = jpr->analog[4];	
		// --start videotöötlus
		imgProc->run();	
		// --end videotöötlus
		
		//timeDif = timer.elapsed();
		//timer.restart();

		//calculate();
		

		//Battery battery(jpr);		
		//std::cout<<"Aku:"<<battery.getBattery()<<std::endl;
		

		//DoSomething();
		//}

		varavaSuund();
		ylemineSuund();
		palliSuund();
		seinaSuund();

		//std::cout<<"vyps: "<< v_suund<<"/"<<y_suund<<"/"<<p_suund<<"/"<<s_suund <<std::endl;
		
		//jpr->setDcMotor(3, 100);
		if(NAEB_PALLI){
			//soidaPallile();
			//std::cout<<"deg: "<< getNurkDeg() <<std::endl;
			//std::cout<<"distH: "<< getBall()->distanceH <<std::endl;
			//printCalcs();
			
			/*std::cout<<"Ball: "<< getBall() <<std::endl;
			imgProc->printBall(imgProc->getBall());
			std::cout<<"BallPrev: "<< getBallPrev() <<std::endl;
			imgProc->printBall(imgProc->getBallPrev());*/
		}//else rattad->omniBreak();
		LiikumisTEST2();
		//sihiNurka(v_suund);
		//soidaPallile5();
	
		//std::cout<<"P_SUUND:"<< p_suund <<std::endl;
		/*std::cout<<"FRAME:"<< getCurFrame()<<"/"<< getCurMs() <<std::endl;
		std::cout<<"P_ANDUR:"<< jpr->analog[1] <<std::endl;
		std::cout<<"V_ANDUR:"<< jpr->analog[0] <<std::endl;
		std::cout<<"VARAV_ANDUR:"<< ylemineV<<"/"<<ylemineP <<std::endl;*/
		//std::cout<<"Aku:"<<battery.getBattery()<<std::endl;
		
		//keeraPalli(v_suund);
		//LiikumisTESTYmberPalli();
		//keeraVaravale();
		//soidaPallile5();		
		//AlmostCompetitionCode0();
		//seinaPall();
		//keeraRingi(v_suund);		
		//PooramisTEST();
		//ymberPalli();
		

		// RULL
		jpr->setDcMotor(3, 100);
	
		// Seina valgustus
		if(SEIN){
			headLightsOn();
		}else headLightsOff();	
		
				/*c++;
		if(c>=2){
			jpr->getView()->show(_img);
			c = 0;
		}
		getCurFrame()++;*/
		if(getCurFrame()%2 ==  0){			
			jpr->getView()->show(imgProc->_img);
		}
		}
		
	}

}

void LiikumisThread::katsetused(){
	int sp=0;
	int m=1;
	while(0){
		setSearchType(st_goal);
		setSearchStatus(ss_searchCircle);
		search2();
		msleep(2000);
		rattad->omniBreak();
		msleep(1000);
	}
	while(0){
		//rattad->omni(0,100,255,0);
		jpr->setDcMotor(2, 100);
		msleep(1000);
		rattad->omniBreak();
		msleep(1000);
	}
	
	while(0){
		rattad->omni(0,255,255,0);
		jpr->setDcMotor(3, 100);
		msleep(1000);
		rattad->omniBreak();
		//jpr->setDcMotor(3, 100);
		msleep(1000);
	}

	while(0){
		jpr->setDcMotor(3, 1);
		msleep(1000);
		jpr->setDcMotor(3, 0);
		msleep(1000);
		jpr->setDcMotor(3, -1);
		msleep(1000);
		jpr->setDcMotor(3, 0);
		msleep(1000);
	}

	while(0){
		rattad->omni(0, 0, 255, 255);
		msleep(400);
		rattad->omniBreak();
		msleep(1000);
	}

	while(0){
		jpr->setDcMotor(0, 255);
		msleep(1000);
		//jpr->setDcMotor(0, -10);
		//msleep(40);
		jpr->setDcMotor(0, 60);
		msleep(1000);	
	}
	while(0){
		std::cout<<"sp: "<<sp<<std::endl;
		//rattad->omni(0,255,255,0);
		jpr->setDcMotor(3, 255);
		//jpr->setDcMotor(1, 255);
		//jpr->setDcMotor(2, sp);
		msleep(500);
		//rattad->omniBreak();
		//jpr->setDcMotor(3, 512);
		//jpr->setDcMotor(1, 512);
		//jpr->setDcMotor(2, 256)
		//msleep(1000);
		if(sp==255)m=-1;
		sp+=m*1;
		if(sp==15)m=1;
		
	}

	while(0){
		jpr->setDcMotor(0, 255);
		jpr->setDcMotor(1, 255);
		jpr->setDcMotor(2, 255);
		sleep(5);
		jpr->setDcMotor(0, 512);
		jpr->setDcMotor(1, 512);
		jpr->setDcMotor(2, 512);
		sleep(5);
	}	
		
	int sek=0;
	while(0){
		if(maga){
			//msleep(500);			
			rattad->omniBreak();
			sek=2;
				
		}else{		
			if(sek>1){			
				sleep(sek);			
				sek=0;
			}
			rattad->omni(0,200,255,0);
			msleep(60);
		}
	}

	while(0){
		jpr->getSensors();
		std::cout<<"0: "<<jpr->analog[0]<<" "<<jpr->analog[1]<<std::endl;
		//std::cout<<"1: "<<jpr->analog[1]<<std::endl;
		
	}

	while(0){
		jpr->getSensors();		
		std::cout<<"P: "<<PALL_EES1<<"/"<<PALL_EES2<<std::endl;
		msleep(50);
	}
}

void LiikumisThread::LiikumisTEST0(){

	if(PALL_EES && !PALL_EES2){
		jpr -> setDcMotor(0, -140);
		jpr -> setDcMotor(1, 140);
		jpr -> setDcMotor(2, 0);

	}else if(PALL_EES && PALL_EES2){
		if(SEIN){
			//std::cout<<"Sein: "<< v_suund << std::endl;			
			keeraRingi(v_suund);			
		}else {

		//kadus = false;
		timer2.restart();

		keeraRingi(v_suund);
		//keeraPalli(v_suund);

			if(VARAV_EES){
				solenoid();
			}
		}
	}else{	
		//if(SEIN){
		//	std::cout<<"Sein"<<std::endl;			
		//	keeraRingi(v_suund);
		//}
		
		if(NURK){
			pallNurgas();
		} else if(NAEB_PALLI){
			std::cout<<"objekt olemas"<<std::endl;			
			//CopyBall();
			//BallPrev = Ball;			
			soidaPallile5();
		}/*else if(NAGI_PALLI && (abs(BallPrev -> distanceH)>50)){
			std::cout<<"objekt oli: "<<BallPrev -> distanceH<<std::endl;
			OtsiViimastPalli();
		}*/else {
			std::cout<<"Ei ole näinud"<<std::endl;			
			//OtsiPalli(p_suund);
		}
		//}
	}

}

void LiikumisThread::LiikumisTEST1()
{
	if(PALL_EES && !PALL_EES2){
		jpr -> setDcMotor(0, -220);
		jpr -> setDcMotor(1, 220);
		jpr -> setDcMotor(2, 0);
	}else
	if(PALL_EES){
		//if(abs(gotPallEesFrame-getCurFrame())<=2){
			//startCircleSearch();
		//}

		if(VARAV_EES){
			solenoid();
			//startCircleSearch();
		}else{
			//search(v_suund);
		}
	}else if(NAEB_PALLI){
		//gotPallEesFrame = getCurFrame();
		if(NURK){
			pallNurgas();
		}else{
			soidaPallile5();
		}		
	}else{
		//gotPallEesFrame = getCurFrame();
		//gotNaebPalliFrame = getCurFrame();
		/*if (abs(gotPallEesFrame-getCurFrame())<=3){
			startCircleSearch();
			endQuickCircleSearch();
		}		
		search(p_suund);*/
	}
}

void LiikumisThread::LiikumisTEST2()
{
	// oli 200 
	if(PALL_EES1 && !PALL_EES2){
		jpr -> setDcMotor(0, -230);
		jpr -> setDcMotor(1, 230);
		jpr -> setDcMotor(2, 0);
	}else
	if(PALL_EES){
		/*if(anduritegaVarav()){
			setSearchStatus(ss_avoidWall);
			setSearchDir(sd_goal);
			search2();	
		}else*/		
		if(NURK || SEIN){
			setSearchType(st_goal);			
			setSearchStatus(ss_avoidWall);
			setSearchDir(sd_goal);
			search2();
		}else
		if ((!V_ANDUR && !P_ANDUR) && VARAV_ANDUR){
			//solenoid();
			setSearchType(st_goal);
			setSearchStatus(ss_avoidWall);
			setSearchDir(sd_ylemine);
			search2();
		}else		
		if(VARAV_EES){
			solenoid();
			setSearchType(st_ball);
			setSearchDir(sd_ball);
			search2();
		}else{
			setSearchType(st_goal);
			setSearchDir(sd_goal);
			search2();
		}
	}else if(NAEB_PALLI){
		/*if(anduritegaVarav()){
			setSearchStatus(ss_avoidGoal);
			setSearchDir(sd_wall);
			search2();	
		}else*/
		if ((!V_ANDUR || !P_ANDUR) && VARAV_ANDUR){
			setSearchStatus(ss_avoidGoal);
			setSearchDir(sd_ylemine);
			search2();
		}else
		if(NURK){
			pallNurgas();
		}else{
			soidaPallile5();
			setSearchType(st_ball);
			setSearchDir(sd_ball);
			//setSearchStatus(ss_avoidWall);
			//setSearchStatus(ss_searchCircle);
		}		
	}else{
		//setSearchType(st_ball);	
		//setSearchDir(sd_ball);	
		search2();
	}
}

void LiikumisThread::LiikumisTESTYmberPalli(){
		
	if(NAEB_PALLI){	
		std::cout<<getBall()->distanceH<<std::endl;
		std::cout<<getBall()->area<<std::endl;
		std::cout<<"Y: "<<getBall()->center.y<<std::endl;
		int nurk = getBall()->distanceH;
		int p=getBall()->distanceH/2;
		int m = 1;
		if(nurk<0)m=-1;
		//nurk *= m;
		
		int reqDist= 5000;
		//double sp = (Ball->area)/reqDist;
		//sp = 1/sp;
		//nurk /= sp;

		/*if((reqDist+4000)<(Ball->area)){
			// tuleb lähemale
			nurk -=m*20;
		}else if((reqDist-2000)>(Ball->area)){
			// läheb kaugemale
			nurk +=m*20;
		}*/
		nurk *= m;

		int speed = 90;
		if(abs(nurk)>abs(speed))nurk = abs(speed); 
		rattad->omni(90, speed, 256, nurk*m);
		//msleep(2000);
	}
}

void LiikumisThread::DoSomething()
{
	
	if(PALL_EES){
		std::cout<<"Pall ees"<<std::endl;
		if(VARAV_EES){
			std::cout<<"Löök"<<std::endl;
			rattad->omniBreak();
			solenoid();
		}else
		if(SEIN){
			std::cout<<"Sein"<<std::endl;
			rattad->omniBreak();
		}else{
			// pall roboti ja värava vahele
			rattad->omniBreak();
		}
	}else{
		if(SEIN){
			std::cout<<"Sein"<<std::endl;			
			rattad -> omniBreak();
		}else
		if(NAEB_PALLI){
			std::cout<<"objekt olemas"<<std::endl;			
			//CopyBall();
			//BallPrev = Ball;			
			soidaPallile();
		}else if(NAGI_PALLI && (abs(getBallPrev() -> distanceH)>50)){
			std::cout<<"objekt oli: "<<getBallPrev() -> distanceH<<std::endl;
			//OtsiViimastPalli();
		}else {
			std::cout<<"Ei ole näinud"<<std::endl;			
			//OtsiPalli(p_suund);
		}
	}
}

void LiikumisThread::soidaPallile(){
	/*class Object {
  	public:    int type; //CAN, SOCK, CAN_BEACON,SOCK_BEACON 
  	CvPoint center; // center coordiantes - kaks int tüüpi muutujat x ja y
  	CvRect rect;        // bounding rectangle neli int x, y, width, height.
  	double area;        // pindala
  	int distanceH;      // horizontal distance from image center
  	};*/
	if(NAEB_PALLI){
		//printBall(getBall());		
		int nd = getNurkDeg()*2;
		if(nd>100)nd=100;
		int sp = getDistY()*3+100;
		if(sp>250)sp=250;
		std::cout<<"spnd: "<<sp<<"/"<<nd<<std::endl;
		rattad -> omni(nd, sp, 255, 0);
	}
}

void LiikumisThread::soidaPallile2()
{
	/*if(NAEB_PALLI){
		double x = laiusToKaugus(getBall()->rect.width);
		std::cout<<x<<std::endl;
		int nurk = xnurk;
		std::cout<<nurk<<std::endl;
		int sp= x;
		int m = 1;
		if(xspeed==0)xspeed=ZERO_SPEED;
		if(speedY()!=0){

			m=1;
			if((sp-speedY())<0)m=-1;
			xspeed = (sp-speedY())*2.0+40*m;
		}
		//xnurk = nurkDeg;
		if(speedN()!=0){

			//m=1;
			//if((speedN()-nurk)<0)m=-1;
			xnurk = (nurkDeg-speedN())*1.0;
		} 
		std::cout<<"SP2: "<<xspeed<<"\t"<<xnurk<<std::endl;
		if(xspeed>250)xspeed = 250;
		if(xspeed<-250)xspeed = -250;
		if(xnurk>250)xnurk = 250;
		if(xnurk<-250)xnurk = -250;
		//if((xspeed>-ZERO_SPEED) && (xspeed<=0))xspeed = -ZERO_SPEED;
		//if((xspeed<ZERO_SPEED) && (xspeed>=0))xspeed = ZERO_SPEED;
		if((xspeed+xnurk)>250)xspeed -=xnurk;
		rattad -> omni(nurkDeg*2, 100, 255, 0);
		std::cout<<"SP3: "<<xspeed<<"\t"<<xnurk<<std::endl;
	}else{
		xspeed = 0;
		xnurk = 0;		
		rattad -> omni(0, xspeed, 255, xnurk);
	}*/
}

void LiikumisThread::soidaPallile3(){
	/*class Object {
  	public:    int type; //CAN, SOCK, CAN_BEACON,SOCK_BEACON 
  	CvPoint center; // center coordiantes - kaks int tüüpi muutujat x ja y
  	CvRect rect;        // bounding rectangle neli int x, y, width, height.
  	double area;        // pindala
  	int distanceH;      // horizontal distance from image center
  	};*/
	/*if(NAEB_PALLI){
		double nd=nurkDeg*2.0;
		double x = calcDist(nd);
		//std::cout<<x<<std::endl;
		//int nurk = getBall()->distanceH/1.5;
		//std::cout<<nurk<<std::endl;
		double sp=65.0+x*3.0;
		if(abs(nurkDeg)<10)sp+=20;
		if(abs(x)>10 && abs(x)<15)sp-=20;

		if((sp>200) || (x>30))sp = 200;
		//if((sp+nurk)>255)sp-=nurk;
		
		if(nd>90)nd=90;if(nd<-90)nd=-90;
		if(x<5)nd/=4;
		rattad -> omni(nd, sp, 255, nurkDeg/2.0);
	}*/
}

void LiikumisThread::soidaPallile4(){
	/*class Object {
  	public:    int type; //CAN, SOCK, CAN_BEACON,SOCK_BEACON 
  	CvPoint center; // center coordiantes - kaks int tüüpi muutujat x ja y
  	CvRect rect;        // bounding rectangle neli int x, y, width, height.
  	double area;        // pindala
  	int distanceH;      // horizontal distance from image center
  	};*/
	/*if(NAEB_PALLI){
		double desiredSpeed = 15; //30 cm/s

		double sp = speedY();
		if(sp<desiredSpeed){
			xspeed += 5+(desiredSpeed-sp)*0.5;//desiredSpeed/2.0+xspeed/10.0;
			if((xspeed>-ZERO_SPEED/2.0) && (xspeed<ZERO_SPEED))xspeed = ZERO_SPEED;
		}else{
			xspeed -= 5+(desiredSpeed-sp)*0.5;//desiredSpeed/2.0+xspeed/10.0;
			if((xspeed<ZERO_SPEED/2.0) && (xspeed>-ZERO_SPEED))xspeed = -ZERO_SPEED;
		}		
		
		double nd=nurkDeg*2.0;
		double x = calcDist(nd);

		//double sp=65.0+x*3.0;
		//if(abs(nurkDeg)<10)sp+=20;
		//if(abs(x)>10 && abs(x)<15)sp-=20;

		if((xspeed>250) )xspeed = 250;
		if(nd>90)nd=90;if(nd<-90)nd=-90;
		//if(x<5)nd/=4;
		if(abs(xspeed+nurkDeg/2.0)>255)xspeed -= nurkDeg/2.0;
		if(xspeed<-150)xspeed = -150;
		//if((sp+nurk)>255)sp-=nurk;
		
		
		rattad -> omni(nd, xspeed, 255, nurkDeg/2.0);
	}else{
		rattad -> omniBreak();
		xspeed = 0;
	}*/
}

void LiikumisThread::soidaPallile5(){
	if(NAEB_PALLI){
		int m = 1;
		int kiirus_z;
		int kiirus_x;
		int aku_konst = 0;
		int seina_konst = 0;

		if((getBall()->distanceH) < 0){
			m=-1;
		}
		//kiire //kiirus_z = (abs(getBall()->distanceH)/1.6) + 32.5;
		// keskmine
		kiirus_z = (abs(getBall()->distanceH) / 3) + 38.3;
		//töötab
		//kiirus_z = (abs(getBall()->distanceH) / 4) + 40;
		//kiirus_z = (40+(abs(getBall()->distanceH)-abs(getBall()->distanceH)*0.4));

/*
		if(aku < 440){
			aku_konst = 0;		
		} else if(aku > 440 && aku < 560) {
			aku_konst = 5;
		}else {
			aku_konst = 10;
		}
*/
		if(getDistY() > 20){				
			kiirus_x = 150;

		} else {					
			kiirus_x = 120;
		}


		if (kiirus_z > 130) {
			kiirus_z = 130;			
		}

		// 10+ juurde
		if(getDistY() < 10 && abs(getBall()->distanceH) > 40){
			jpr -> setDcMotor(0, m*(kiirus_z));
			jpr -> setDcMotor(1, m*(kiirus_z));
			jpr -> setDcMotor(2, m*(kiirus_z));
		} else {

			int s_vasak = jpr->analog[0];
			int s_parem = jpr->analog[1];

			if(getDistY() < 10){

				//jpr -> setDcMotor(0, m*(kiirus_z));
				//jpr -> setDcMotor(1, m*(kiirus_z));
				if((s_vasak < 200) && (s_parem > 300)){
					jpr -> setDcMotor(2, 4*(kiirus_z));
				} else if ((s_vasak > 300) && (s_parem < 200)){
					jpr -> setDcMotor(2, -4*(kiirus_z));
				} else {
					rattad->omni(0, kiirus_x, 255, m*(kiirus_z-(30+aku_konst)));
				}			

			} else {			
			// > 20 = 150 else 110
			// kiirus_z piir 130
			// 30 +aku_konst
			rattad->omni(0, kiirus_x, 255, m*(kiirus_z-(30+aku_konst)));
			//jpr -> setDcMotor(0, kiirus_x);
			//jpr -> setDcMotor(1, kiirus_y);
			//jpr -> setDcMotor(2, kiirus_z);
			}
		}
	}

}

void LiikumisThread::soidaPallile6(){
	if(NAEB_PALLI){
		

		//rattad->omni(dir,kiirus,255,nurk);
	}
}


void LiikumisThread::solenoid(){
	//abstract robot meetod sendCommand(162+motor)
	//jpr->getCom()->sendCommand(165, 255);
	//jpr->getCom()->sendCommand(165, 0);
	//rattad->omni(0,0,256,0);
	//jpr->setDcMotor(3, 255);
	
	if(isSolenoidReady()){
		jpr->setDigital(0);	
		msleep(10);
		jpr->clearDigital(0);

		kickFrame = getCurFrame();
		kickMs = getCurMs();
	}

}
// 2,5-15cm
// 2,5 - 155; 5 - 118; 10 - 79; 15 - 58;

/*double LiikumisThread::laiusToKaugus(int x){
	if(x < 10){
		x = 10;
	} else if (x > 100){
		x = 100;
	}

	int z = x;
	return (1.34689593735949e-08)*(z*z -z*183.799 +8454.955)*(z*z -z*48.415 +879.989)*(z*z -z*125.759 +4414.899);
}*/

/*double LiikumisThread::getYcm(Image::Object *B)
{
	double x = B->rect.y;
	if(x>150)x=150;
	if(x<5)x=5;
	//191,5;173,7;149,10;137,12;121,15;106,20;95,25;78,35;67,50;46,150;
	return 1.4878394321544e-15*(x -16.222)*(x*x -402.953*x +40651.332)*(x*x -340.259*x +29660.601)*(x*x -136.377*x +5410.666)*(x*x -241.514*x +15907.208);
}

double LiikumisThread::getYwidth(Image::Object *B)
{
	double x = B->rect.y;
	if(x>150)x=150;
	if(x<5)x=5;
	//191,95;173,82;149,71;137,62;121,53;106,43;95,35;78,26;20,50;46,8;
	return 2.0353917306713e-15*(x +7.518)*(x*x -75.301*x +1481.087)*(x*x -381.863*x +36650.468)*(x*x -161.136*x +7663.185)*(x*x -286.475*x +22047.054);
}*/


//#define VARAV_EES (IR_V_ANDUR && IR_P_ANDUR)
//#define VARAV_VASAKUL (IR_V_ANDUR && !IR_P_ANDUR)
//#define VARAV_PAREMAL (IR_P_ANDUR && !IR_V_ANDUR)
//#define VARAV (VARAV_EES || VARAV_VASAKUL || VARAV_PAREMAL)

void LiikumisThread::keeraVaravale(){
	if(NAEB_PALLI){

		int PALLI_KAUGUS = 6; 

		int kaugus = getDistY();//laiusToKaugus(getBall()->rect.width);
		//int kesk_kaugus = std::abs(getBall()->distanceH) - std::abs(getBall()->distanceH/4);
		int k_konst = (12-kaugus)*2;

		int m = -1;
		int suund = -1;
		int nurk = 0;
		int stop = 1;
		int kesk_kaugus = getBall()->distanceH - suund*std::abs(getBall()->distanceH/4);

		if(VARAV_PAREMAL){
			//kesk_kaugus = std::abs(getBall()->distanceH) - std::abs(getBall()->distanceH/4);
			suund = -1;
			m = 1;
		}else if(VARAV_VASAKUL){
			//kesk_kaugus = std::abs(getBall()->distanceH) - std::abs(getBall()->distanceH/4);
			suund = 1;
			m = -1;
		}else if(VARAV_EES){	
			stop = 0;
		}else {
			stop = 1;
		}

		//kauguse hoidmine
		if(PALLI_KAUGUS < kaugus){
			nurk = -5;
		}else{
			nurk = 5;
		}
		//nurk *= suund;
		//kesk_kaugus *= suund;
		//k_konst *= suund;

		//rattad->omni(90+nurk, stop*100+k_konst*suund, 255, m*50+kesk_kaugus+k_konst);

		//std::cout<<"\n"<< getBall()->rect.width <<"\n"<< kesk_kaugus <<std::endl<<std::endl;


		//kesk_kaugus -140
		int bn=suund*(90+nurk);
		int sp = stop*((100+k_konst));
		int n = stop*((-1)*(suund*50-kesk_kaugus+k_konst));
		rattad->omni(bn, sp, 255, n);
		//rattad->omni(90+nurk, stop*(suund*(100+k_konst)), 255, stop*(m*(50+tmp+k_konst)));
		
		std::cout<<bn<<"/"<<sp<<"/"<<n<<std::endl;


	}
}

void LiikumisThread::varavaSuund(){

	if(VARAV_VASAKUL){
		v_suund = -1; 	
	} else if (VARAV_PAREMAL){
		v_suund = 1;
	}

}

void LiikumisThread::ylemineSuund(){

	if(getYlemineV()>getYlemineP()){
		y_suund = 1; 	
	} else{
		y_suund = -1;
	}

}


void LiikumisThread::sihiNurka(int suund){
	if(PALL_EES){
		if(VARAV_EES){
			while (VARAV_EES){
				jpr->setDcMotor(0, suund*100);
				jpr->setDcMotor(1, suund*100);
				jpr->setDcMotor(2, suund*100);
			}
			solenoid();
		}		

		//rattad->omni(0, 90, 255, 70);		
		jpr->setDcMotor(0, suund*100);
		jpr->setDcMotor(1, suund*100);		jpr->setDcMotor(2, suund*100);			
		
	} 

	/*else {
		rattad -> omniBreak();
	}*/
}


void LiikumisThread::keeraPalli(int suund){
	if(PALL_EES){
		std::cout<<"pall ees"<<"/"<<std::endl;
		if(VARAV_EES){
			rattad -> omniBreak();
		} else if (VARAV_PAREMAL){
			suund = 1;				
		} else if (VARAV_VASAKUL){
			suund = -1;
		} else {
			//rattad->omni(0, 90, 255, 70);
			jpr->setDcMotor(0, -100);
			jpr->setDcMotor(1, 100);
			jpr->setDcMotor(2, 230*suund);
		}
	} 

	/*else {
		rattad -> omniBreak();
	}*/
}

void LiikumisThread::AlmostCompetitionCode0(){	
	if(PALL_EES){
		//rattad->omni(0, 90, 255, 70);
		// -100/100/250 täisakuga ei toimi hästi
		// -100/100/230
		//kadus = false;
		timer2.restart();

		/*jpr->setDcMotor(0, -100);
		jpr->setDcMotor(1, 100);
		jpr->setDcMotor(2, 230);
		*/
		
		keeraPalli(v_suund);

		if(VARAV_EES){
			solenoid();
		}
	}else{
		/*if(SEIN){
			std::cout<<"Sein"<<std::endl;			
			rattad -> omniBreak();
		}else*/
		/*if(kadus){
			Hark();
		//	rattad->omni(0,140,255,0);
		//	msleep(500);kadus = false;			
			if(timer2.elapsed()>2000){
				kadus = false;
				timer2.restart();
			}
		}else{*/
			if(NAEB_PALLI){
				std::cout<<"objekt olemas"<<std::endl;			
				//CopyBall();
				//getBallPrev() = getBall();			
				soidaPallile3();
			}/*else if(NAGI_PALLI && (abs(getBallPrev() -> distanceH)>50)){
				std::cout<<"objekt oli: "<<getBallPrev() -> distanceH<<std::endl;
				OtsiViimastPalli();
			}*/else {
				std::cout<<"Ei ole näinud"<<std::endl;			
				//OtsiPalli(p_suund);
			}
		//}
	}
}

void LiikumisThread::PooramisTEST(){
	/*if(NAEB_PALLI && distY<50){
		double x = distX;
		int m = 1;
		if(x<0)m = -1;
		if(abs(x)>250){
			x=m*250;
		}
		std::cout<<"xx: "<<x<<std::endl;
		rattad->omni(0, 0, 255, (x+m*40));
		std::cout<<"ZZ: "<<nurkDeg<<"/"<<x+m*50<<"/"<<distX<<std::endl;
	}else{
		rattad->omniBreak();
	}*/
}

/*double LiikumisThread::speedY(){
	if(timeDif == 0)timeDif = 1;
	return distYDif/(timeDif/1000.0);
}

double LiikumisThread::speedX(){
	if(timeDif == 0)timeDif = 1;
	return distXDif/(timeDif/1000.0);
}

double LiikumisThread::speedN(){
	if(timeDif == 0)timeDif = 1;
	return xnurkDif/(timeDif/1000.0);
}

double LiikumisThread::laiusToXcm(Image::Object *B)
{
	//if(B->rect.width==0)
	return (B->distanceH)/((B->rect.width)/4.28);
}*/

/*double LiikumisThread::calcNurk(){
	if(NAEB_PALLI){
		nurkRad = atan(distX/distY);		
		nurkDeg = nurkRad/0.0174532925;
	}
	return nurkDeg;
}*/

/*double LiikumisThread::calcDist(double deg){
	double dx=sin(deg*0.0174532925)*getDistX();
	double dy=cos(deg*0.0174532925)*getDistY();
	return sqrt(dx*dx+dy*dy);
}*/

/*void LiikumisThread::seinaPall(){
	if(PALL_EES){
		if(SEIN_EES){
			//rattad->omni(0,0,255,200);
			keeraPalli(v_suund);
		}else
		if(SEIN_VASAKUL){
			//rattad->omni(0,0,255,200);
			keeraPalli(v_suund);
		}else
		if(SEIN_PAREMAL){
			//rattad->omni(0,0,255,-200);
			keeraPalli(v_suund);
		}
	}else{
		rattad->omniBreak();
	}
}*/

void LiikumisThread::keeraRingi(int suund){
	if(PALL_EES){
	std::cout<<"suund: "<< suund<<std::endl;

		//rattad->omni(0, 100, 255, 90);	
		jpr->setDcMotor(0, suund*100);
		jpr->setDcMotor(1, suund*100);
		jpr->setDcMotor(2, suund*100);			
	}
}

void LiikumisThread::pallNurgas(){
	if(NURK && NAEB_PALLI && !PALL_EES2){
		if(abs(kickFrame-getCurFrame())>45){
		//if(abs(kickMs-getCurMs())<=(45*40)){
			solenoid();
		}
	}
}

void LiikumisThread::palliSuund(){
	if(NAEB_PALLI && isSolenoidReady()){		
		if(getBall()->distanceH<0){
			p_suund = -1;
		} else {
			p_suund = 1;
		}
	}
}

void LiikumisThread::seinaSuund(){
	if(SEIN){
		if((jpr->analog[0]) > (jpr->analog[1])){
			s_suund = -1;
		} else {
			s_suund = 1;
		}
	}
}

/*void LiikumisThread::printBall(Image::Object *B)
{

	std::cout<<"----------------------------"<<std::endl;
	std::cout<<"BallFromFrame: "<<getCurFrame()<<std::endl;
	std::cout<<"center.x: "<<B->center.x<<std::endl;
	std::cout<<"center.y: "<<B->center.y<<std::endl;
	std::cout<<"rect.x: "<<B->rect.x<<std::endl;
	std::cout<<"rect.y: "<<B->rect.y<<std::endl;
	std::cout<<"rect.width: "<<B->rect.width<<std::endl;
	std::cout<<"rect.height: "<<B->rect.height<<std::endl;
	std::cout<<"area: "<<B->area<<std::endl;
	std::cout<<"distanceH: "<<B->distanceH<<std::endl;
}*/

void LiikumisThread::search2(){
	int deg = 0;
	int avgAndur = 0;
	int suund = 1;
	switch(searchDir){
		case sd_minus:suund = -1;break;
		case sd_plus:suund = 1;break;
		case sd_ball:suund = p_suund;break;
		case sd_goal:suund = v_suund;break;
		case sd_ylemine: suund= y_suund;break;
		case sd_wall:suund = s_suund;break;
	}	
	switch(searchStat){
		case ss_avoidWall:
			//if(abs(searchFrame-getCurFrame())<=10){
			if(abs(searchMs-getCurMs())<=(10*40)){
				rattad->omni(0,-200,255,0);	//tagurdab
			}else{
				switch(searchTyp){
				case st_ball: setSearchStatus(ss_quickTurn);break;
				case st_goal: setSearchStatus(ss_searchCircle);break;
				}
			}			
			break;
		case ss_avoidGoal:
			//if(abs(searchFrame-getCurFrame())<=10){
			if(abs(searchMs-getCurMs())<=(10*40)){
				rattad->omni(180,200,255,suund*50);	//tagurdab pöörab
				std::cout<<"ag: "<<suund*50<<std::endl;
			}else{
				setSearchStatus(ss_forward);
			}
			break;
		case ss_avoidRobot:
			break;
		case ss_searchCircle:
			//if(abs(searchFrame-getCurFrame())<=90){
			if(abs(searchMs-getCurMs())<=(90*40)){
				rattad->omni(0, 0,255,suund*90);	// pööra kohapeal
			} else {
				setSearchStatus(ss_forward);
			}
			break;
		case ss_quickTurn:			
			//if(abs(searchFrame-getCurFrame())<=14){
			if(abs(searchMs-getCurMs())<=(14*40)){
				rattad->omni(0,0,255,suund*120);
			}else{
				rattad->omniBreak();				
				setSearchStatus(ss_forward);
			}			
			break;
		case ss_forward:
			if(SEIN){
				rattad->omniBreak();
				setSearchStatus(ss_avoidWall);
			}else if ((!V_ANDUR || !P_ANDUR) && VARAV_ANDUR){
				rattad->omniBreak();
				setSearchStatus(ss_avoidWall);
			}else			
			//if(abs(searchFrame-getCurFrame())<=90){
			if(abs(searchMs-getCurMs())<=(90*40)){
				//if(getCurFrame()%20 == 0)searchM *= -1;
				//deg = searchM*2*(getCurFrame()%20+1);
				//avgAndur = (jpr->analog[0] + jpr->analog[1])/2;
				//avgAndur /= 2;			
				//rattad->omni(0, 200-abs(deg),255,deg);	// tuigerdades edasi
				//rattad->omni(deg, 200,255,deg);	// tuigerdades edasi
				//std::cout<<"avg: "<<avgAndur<<"/"<<(jpr->analog[0])<<"/"<<jpr->analog[1]<<std::endl;
				//if(avgAndur>=70){
				//	rattad->omni(0,100,255,0);
				//}else{
					rattad->omni(0,200,255,0);
				//}
			}else{
				rattad->omniBreak();
				setSearchStatus(ss_searchCircle);
			}
			break;
		default:
			break;
	}
}

void LiikumisThread::setSearch2(ESearchStatus stat, ESearchType typ){
	if((stat != searchStat) || (typ != searchTyp)){
		searchFrame = getCurFrame();
		searchMs = getCurMs();
		searchStat = stat;
		searchTyp = typ;
	}
}

void LiikumisThread::setSearchStatus(ESearchStatus stat){
	if(stat != searchStat){
		searchFrame = getCurFrame();
		searchMs = getCurMs();
		searchStat = stat;
	}
}

void LiikumisThread::setSearchType(ESearchType typ){
	if(typ != searchTyp){
		searchFrame = getCurFrame();
		searchMs = getCurMs();
		searchTyp = typ;
		searchStat = ss_searchCircle;
	}
}

void LiikumisThread::setSearchDir(ESearchDir dir){
	searchDir = dir;
}

Image::Object* LiikumisThread::getBall(){
	if(imgProc != NULL){
		return imgProc->Ball;
	}else return NULL;
}

Image::Object* LiikumisThread::getBallPrev(){
	if(imgProc != NULL){
		return imgProc->BallPrev;
	}else return NULL;
}

int LiikumisThread::getCurFrame()
{
	if(imgProc != NULL){	
		return imgProc->curFrame;
	}else return 0;
}

int LiikumisThread::getUpdated(){
	if(imgProc != NULL){
		return imgProc->isUpdated();
	}else return NULL;
}

void LiikumisThread::headLightsOn(){
	jpr->setDigital(3);
}

void LiikumisThread::headLightsOff(){
	jpr->clearDigital(3);
}

int LiikumisThread::getCurMs()
{
	return masterTimer.elapsed();
}

void LiikumisThread::printCalcs(){
	
	std::cout<<"timeDif: "<<getTimeDif()<<std::endl;
	std::cout<<"getDistX: "<<getDistX()<<std::endl;
	std::cout<<"getDistY: "<<getDistY()<<std::endl;
	std::cout<<"getDist: "<<getDist()<<std::endl;
	//std::cout<<"getNurk: "<<getNurk()<<std::endl;
	std::cout<<"getDistXDif: "<<getDistXDif()<<std::endl;
	std::cout<<"getDistYDif: "<<getDistYDif()<<std::endl;
	std::cout<<"getNurkDif: "<<getNurkDif()<<std::endl;
	std::cout<<"getSpeedX: "<<getSpeedX()<<std::endl;
	std::cout<<"getSpeedY: "<<getSpeedY()<<std::endl;
	std::cout<<"getSpeed: "<<getSpeed()<<std::endl;
	std::cout<<"getSpeedN: "<<getSpeedN()<<std::endl;
	std::cout<<"getNurkRad: "<<getNurkRad()<<std::endl;
	std::cout<<"getNurkDeg: "<<getNurkDeg()<<std::endl;
}

void LiikumisThread::ymberPalli(){
	if(NAEB_PALLI){
		double dir = 90;
		double kiirus = 90;
		double nurk = getNurkDeg();
		if(getDistY()>10){
			dir = 70;
		}else if(getDistY()<8){
			dir = 110;
		}
		//if(dir == 90)
		dir *= -s_suund;
		rattad->omni(dir,kiirus,255,nurk);
	}

		/*int nurk = getBall()->distanceH;
		int p=getBall()->distanceH/2;
		int m = 1;
		if(nurk<0)m=-1;
		//nurk *= m;
		
		int reqDist= 5000;
		//double sp = (Ball->area)/reqDist;

		nurk *= m;

		int speed = 90;
		if(abs(nurk)>abs(speed))nurk = abs(speed); 
		rattad->omni(90, speed, 256, nurk*m);
		//msleep(2000);*/
}

void LiikumisThread::mainKickSolenoid(){
	if(PALL_EES && VARAV_EES && !(NURK || SEIN) /*&& !((!V_ANDUR && !P_ANDUR) && VARAV_ANDUR)*/){
		solenoid();
	}
}

bool LiikumisThread::anduritegaVarav(){
	if((!V_ANDUR || !P_ANDUR) && YLEMINEV_ANDUR && YLEMINEP_ANDUR){
		return true;
	}else return false;
}

bool LiikumisThread::anduritegaRobot(){
	if((YLEMINEV_ANDUR && V_ANDUR) || (YLEMINEP_ANDUR && P_ANDUR)){
		return true;
	}else return false;
}

bool LiikumisThread::isSolenoidReady(){
	return abs(kickMs-getCurMs())>1000;
}


