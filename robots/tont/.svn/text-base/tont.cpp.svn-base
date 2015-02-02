//
// C++ Implementation: tont
//
// Description: 
//
//
// Author: 
//
// Copyright: See COPYING file that comes with this distribution
//
//
// Koodi stiil: 1 aste 4 space-i (gedit-is: Tab Width: 4, use spaces ON), muu pmst java koodistandardi järgi.
//
#include "tont.h"
#include <sys/time.h>
#include <QTime>
#include <QDebug>
#include <math.h>
#include <iostream>


#define MAX_KIIRUS 220 //maksimaalne kiirus, millega antud koodis s6ita (tuleb muuta veel)
#define MIN_KIIRUS_KP 55

#define BALL 1
#define GOAL_RED 2
#define GOAL_BLUE 3

#define PALL_PAREMAL 1
#define PALL_VASAKUL 2
#define PALL_OTSE 3
#define PALL_L2HEDAL_P 4
#define PALL_L2HEDAL_V 5
#define PALL_L2HEDAL_OTSE 6

#define V2RAV_PAREMAL 1
#define V2RAV_VASAKUL 2
#define V2RAV_OTSE 3
#define V2RAV_TAGA 4

#define V2RAV_KITSAS 1
#define V2RAV_KESKMINE 2
#define V2RAV_LAI 3

#define V2RAV_K6RVAL 1
#define V2RAV_L2HEDAL 2
#define V2RAV_KAUGEL 3


#define SEIN_K6RVAL 1
#define SEIN_L2HEDAL 2
#define SEIN_KAUGEL 3




using namespace std;

QTime delay;

Tont::Tont() //konstruktor
{
}
Tont::~Tont()//läheb käima siis, kui programm kinni läheb
{
}

//FUNKTSIOONID
//p88ramine, kui mootoritele vaja anda 2 v22rtust
void Tont::p88ra(int vasak, int parem)
	{ //mootorite kiirus, liikumine

	setDcMotor(0, vasak);
	setDcMotor(1, parem);
	}

//liikumine, kui mootoritel sama v22rtus
void Tont::otse()
	{	
	setDcMotor(0, otseKiirus - vMootorVaheEdasi);
	setDcMotor(1, otseKiirus);
	}

void Tont::tagurda()
	{
	setDcMotor(0, tahaKiirus + 255);
	setDcMotor(1, tahaKiirus + 255 - pMootorVaheTagasi);
	}

void Tont::liigu(int kiirus)
	{	

	setDcMotor(0, kiirus);
	setDcMotor(1, kiirus);
	}

//kohapeal p88re paremale
void Tont::KPp88reP()
	{
	setDcMotor(0, kiirusKP);
	setDcMotor(1, kiirusKP + 255);
	//std::cout<<" teen KPp88retP"<<std::endl;
	}

//kohapeal kiirendusega p88re paremale
void Tont::KPkiirendP() 
	{
	if(v2ravaOtsimKiirusKP - algkiirusKP > 10) 
		{
			if(kiiruseT6stmineTimer.elapsed()>100)
				{
				algkiirusKP += 10;	
				kiiruseT6stmineTimer.restart();
				algkiirusKP += 5;	
				setDcMotor(0, algkiirusKP);
				setDcMotor(1, algkiirusKP + 255);
				}		
		}
		else 
		{
			setDcMotor(0, v2ravaOtsimKiirusKP);
			setDcMotor(1, v2ravaOtsimKiirusKP + 255);
			//algkiirusKP = MIN_KIIRUS_KP;
		}	
	} //KPkiirendP l6pp

//kohapeal kiirendusega p88ramiseks vasakule
void Tont::KPkiirendV()
	{
	if(v2ravaOtsimKiirusKP - algkiirusKP > 10) 
		{
			if(kiiruseT6stmineTimer.elapsed()>100)
				{
				algkiirusKP += 10;	
				kiiruseT6stmineTimer.restart();
				setDcMotor(0, algkiirusKP + 255);
				setDcMotor(1, algkiirusKP);
				}
		
		}
		else 
		{
			setDcMotor(0, v2ravaOtsimKiirusKP + 255);
			setDcMotor(1, v2ravaOtsimKiirusKP);
			//algkiirusKP = MIN_KIIRUS_KP;
		}	
	}//KPkiirendV l6pp

//pidurdamine
void Tont::pidur() 
	{
	setDcMotor(0, 512);
	setDcMotor(1, 512);
	}

void Tont::eemaldu()
	{
	tagurda();
	msleep(250);
	p88ra(255, 510);
	msleep(200);
	palliPyydmisTimer.restart();
	seinaTimer.restart();
	pallN2ha = false;
	clearBuffer();
	getAnalogs();
	}

//kaamerapildi lag'i v2hendamiseks p2rast sleep fnc kasutamist
void Tont::clearBuffer()
	{
	for (int i = 0; i < 6; i++) image->getFrame();
	}

//palli löömine
void Tont::kickBall()
	{ 
	pidur(); //korra seisma enne l88mist
	if(l88giTimer.elapsed() > 1750)
		{
		setDigital(2);
		msleep(200);
		clearDigital(2);
		l88giTimer.restart();
		otsimine.restart();
		sihibKeskeleTimer.restart();
		getAnalogs();
		pallK2es = false;
		pallN2ha = false;
		pallL2hedal = false;
		sihibKeskeleTimerT88tab = false;
		}
	clearBuffer();
	}

void Tont::otsiPall() 
	{
	if(!palliTimerT88tab) 
		{
		otsimine.start();
		palliTimerT88tab = true;
		}
	if(otsimine.elapsed()<5700 || seinaKaugus != SEIN_KAUGEL || v2ravaKaugus != V2RAV_KAUGEL ) 
		{ 
		//std::cout<<"KPeP ja otsiPalli"<<std::endl;
		KPp88reP();
		}
	if(otsimine.elapsed()>=5700 && otsimine.elapsed()<6800 && seinaKaugus == SEIN_KAUGEL) 
		{
		otseKiirus = 255;
		otse();
		}
		if(otsimine.elapsed() >= 6800)
		{
		otseKiirus = MAX_KIIRUS;
		otsimine.restart(); 
		}	
	}//otsiPall l6pp


//S6IT PALLILE, MIS ON NII L2HEDAL, ET KAOB KOHE KAAMERAPILDIST JA ON OTSE
		void Tont::otseL2hiPallile()
		{
			if(pallL2hedalTimer.elapsed() < 1500 && !pallK2es) //s6idab otse, kuni sekund saab t2is v6i kuni pall on pesas
				{ 
				liigu(otseKiirusL2hedal);
				//getAnalogs();	
				}
						
			if(pallK2es||pallL2hedalTimer.elapsed() > 1500) 
				{
				pallL2hedal = false;
				//std::cout<<"l2hedal vale"<<std::endl;
				}
		}//otseL2hiPallile l6pp

//P88RE VASAKUL KAUGEMAL ASUVALE PALLILE
	void Tont::p88reKaugPallileV() 
		{
		p88ra(kiirusKaugPallileV2ike, kiirusKaugPallileSuur);
		}
//P88RE PAREMAL KAUGEMAL ASUVALE PALLILE
	void Tont::p88reKaugPallileP()
		{
		p88ra(kiirusKaugPallileSuur, kiirusKaugPallileV2ike);
		}

//P88RE PAREMAL L2HEDAL ASUVALE PALLILE
	void Tont::p88reL2hiPallileP() 
		{
		p88ra(kiirusL2hiPallileSuur, kiirusKaugPallileV2ike);
		}

//P88RE VASAKUL L2HEDAL ASUVALE PALLILE
	void Tont::p88reL2hiPallileV()
		{
		p88ra(kiirusL2hiPallileV2ike, kiirusL2hiPallileSuur);
		}

	void Tont::otsiV2rav() {
	//V2RAVA OTSIMINE
//teha ymber otsimise kood, kui ei n2e v2ravat - TODO - kiiremaks ja selliseks, et ei k2iks yhtemoodi kogu aeg!

			if(v2ravaSignaal == V2RAV_PAREMAL) //V2RAV_TAGA KASUTAMATA PRAEGU
				{
				KPkiirendP();
				//std::cout<<"p88ran paremale"<<std::endl;
				}
			if(v2ravaSignaal == V2RAV_VASAKUL)
				{
				KPkiirendV();
				//std::cout<<"p88ran vasakule"<<std::endl;
				}
				//KUI  PALL ON PESAS JA V2RAVAT EI LEIA 6 sekundiga
			if(palliPesaTimer.elapsed() > 5500 && seinaKaugus == SEIN_KAUGEL) 
				{
					liigu(255);
				}
			if(palliPesaTimer.elapsed() > 7000) 
				{
					palliPesaTimer.restart();
				}

//TODO kui v2rava pindala on suur, ei pea distanceH olema suur. Kui v2ravat on v2ga v2he n2ha - ligemale s6ita. 
//TODO Kui v2rava distance on suurem - kiiremini p88rata, kui v2iksem - aeglasemalt


	}//otsiV2rav l6pp

//P88RAMINE L2HEDAL VASAKUL ASUVA V2RAVA POOLE
	void Tont::p88reL2hiV2ravaleV()
		{
		if(v2rav.distanceH > -80 && v2rav.distanceH < -30) 
			{
			p88ra(kiirusKeskV2ravale + 255, kiirusKeskV2ravale);
			}
		else 
			{
			p88ra(kiirusL2hiV2ravale + 255, kiirusL2hiV2ravale);
			}
		}//p88reL2hiV2ravaleV l6pp

//P88RAMINE L2HEDAL PAREMAL ASUVA V2RAVA POOLE
	void Tont::p88reL2hiV2ravaleP()
		{
		if(v2rav.distanceH < 80 && v2rav.distanceH > 30) 
			{
			p88ra(kiirusKeskV2ravale, kiirusKeskV2ravale + 255);
			}
		else 
			{
			p88ra(kiirusL2hiV2ravale, kiirusL2hiV2ravale + 255);
			}
		}//p88reL2hiV2ravaleP l6pp

//P88RAMINE KAUGEL VASAKUL ASUVA V2RAVA POOLE
	void Tont::p88reKaugV2ravaleV()
		{
		p88ra(kiirusKaugV2ravale + 255, kiirusKaugV2ravale);
		}

//P88RAMINE KAUGEL PAREMAL ASUVA V2RAVA POOLE
	void Tont::p88reKaugV2ravaleP()
		{
		p88ra(kiirusKaugV2ravale, kiirusKaugV2ravale + 255);
		}
//P88RAMINE KESKMISELT KAUGEL VASAKUL ASUVA V2RAVA POOLE
	void Tont::p88reKeskV2ravaleV()
		{
		if(v2rav.distanceH > -100) 
			{
			p88ra(kiirusL2hiV2ravale + 255, kiirusL2hiV2ravale);
			}
		else 
			{
			p88ra(kiirusKeskV2ravale + 255, kiirusKeskV2ravale);
			}
		}//p88reKeskV2ravaleV l6pp
//P88RAMINE KESKMISELT KAUGEL PAREMAL ASUVA V2RAVA POOLE
	void Tont::p88reKeskV2ravaleP()
		{
		if(v2rav.distanceH < 100) 
			{
			p88ra(kiirusL2hiV2ravale, kiirusL2hiV2ravale + 255);
			}
		else 
			{
			p88ra(kiirusKeskV2ravale, kiirusKeskV2ravale + 255);
			}
		}//p88reKeskV2ravaleP l6pp

void Tont::go() {
//TODO - sihibkeskeletimer
    //struct timeval a,b;
    Config  & conf  = Config::getConfig();

    qDebug("Tont Start...\n");


    Image::Object * obj;

    conf.setSendCmdEnabled(1);//mikrocontr-ga suhtlus hakkab sellest.

	int v2ravaTyyp; 
std::cout<<"kirjuta v2rava nr - 0 on punane ja 1 on sinine"<<std::endl;
	std::cin >> v2ravaTyyp;
	if(v2ravaTyyp == 0) {
		v2ravaTyyp = GOAL_RED;
	}
	if(v2ravaTyyp == 1) {
		v2ravaTyyp = GOAL_BLUE;
	}

	palliTimerT88tab = false;
	v2ravaTimerT88tab = false;
	pallK2es = false;
	pallL2hedal = false;
	pallN2ha = false;
	v2ravN2ha = false;
	palliOtsimisTimerT88tab = false;
	sihibKeskeleTimerT88tab = false;

	palliAsukoht = 0; //palli asukoht roboti kaamerapildi keskpunkti suhtes
	v2ravaAsukoht = 0;
	v2ravaSignaal = V2RAV_PAREMAL;
	v2ravaLaius = 0; //v2rava laius roboti pildis - lai/kitsas/keskmine
	v2ravaKaugus = V2RAV_KAUGEL;
	seinaKaugus = SEIN_KAUGEL;

	otseKiirus = 220; //mootori kiiruse muutmiseks otses6idul
	tahaKiirus = 255; //mootori kiiruse muutmiseks tagurdamisel
	otseKiirusL2hedal = 80; //mootori kiiruse muutmiseks otses6idul palli l2hedal

	//kui kiirus on 200, siis mootorite vahe u 10. kui kiirus on 255, siis mootorite vahe vasakul u 18 ja paremal u 12 vms. 
	//palli otsimise p88rlemiskiirus 100
	//v2rava otsimise p88rlemiskiirus ...
	v2ravaOtsimKiirusKP = 110;
	kiirusKP = 110; //kohapeal p88ramise kiirus 
	algkiirusKP = 60; //kiirendusega p88ramise algkiirus	
	vMootorVaheEdasi = 15; //mootori kiiruste vahe - vasakust tuleb lahutada
	pMootorVaheTagasi = 15; //mootori kiiruste vahe - paremast tuleb lahutada


	//kui objekt on n2htaval, siis selle poole suunamiseks - kui pall on paremal, siis vasaku mootori v22rtus on suurem, kui pall, siis vastupidi
	kiirusKaugPallileSuur = 220; //suurem mootori kiirus, p88rates kaugemal asuvale pallile
	kiirusKaugPallileV2ike = 120; //v2iksem mootori kiirus, p88rates kaugemal asuvale pallile
	kiirusL2hiPallileSuur = 150; //suurem mootori kiirus, p88rates l2hedal asuvale pallile
	kiirusL2hiPallileV2ike = 50; //v2iksem mootori kiirus, p88rates l2hedal asuvale pallile

	kiirusL2hiV2ravale = 90; //kiirus, millega p88ratakse kohapeal v2rava poole, mis on l2hedal - kui testimine n2itab, et v6ib j22da sama, mis kiirusKP - pole vaja eraldi muutujat
	kiirusKaugV2ravale = 50; //kiirus, millega p88ratakse kaugemal asuva v2rava poole
	kiirusKeskV2ravale = 60; //kiirus, millega p88ratakse kaugemal asuva v2rava poole
		

	pildiKaunter = 0;
	
	otsimine.start();
	l88giTimer.start(); //timer, mis kontrollib seda, kui tihti v6ib palli lyya
	pildiReopenTimer.start();
	palliPesaTimer.start(); //timer, mis kontrollib, et pall oleks olnud mingi aja pesas enne löömist
	pallL2hedalTimer.start();
	kiiruseT6stmineTimer.start();
	seinaTimer.start();
	sihibKeskeleTimer.start();



//WHILE ALGAB SIIT


	while(1)
		{
//VAJALIKE V22RTUSTE ALGV22RTUSTAMINE
	//maxObj - jahitav pall
			maxObj.area = 0;
			maxObj.distanceH = 0;
			maxObj.type = -1;

		//v2rav - jahitav v2rav	
			v2rav.area = 0;
			v2rav.rect.width = 0;
			v2rav.distanceH = 0;
			v2rav.type = -1;

			v2ravaAsukoht = 0;
			//v2ravaSignaal = 0;

//PILDIT88TLUS		
		cvGrabFrame(image->capture);
		_img = image->getFrame(); //v6etakse kaamerast pilt
		 image->process(1,0,0,0); //pilt protsessitakse (leitakse objektid)

			//videomode 12 ja ylevalt n2htavate pixlite arv!
		 if (image->found_objects != NULL) 
			{

			
		
		  	for (int i = 0; i < image->found_objects->total; i++) 
				{ //k2ib k6ik leitud objektid l2bi

    			obj =  (Image::Object* ) cvGetSeqElem( (image->found_objects), i ); //parajasti t88deldav objekt
      	
      			if(obj!=0) //kui on n2ha m6nda objekti
					{
					if(!pallK2es) 
						{
						if(obj->type==BALL)
						{
					//myra eemaldamine - minimaalne palli pindala vastavalt palli kaugusele
							if(obj->area > maxObj.area && ((obj->area > 35 && obj->rect.y < 100) || 
										(obj->area > 500 && obj->rect.y >=100))) 
							{ 
		
								maxObj = *obj;
							}


//kui uus objekt ei ole sama, mis maxObjekt, aga on rohkem keskel ja mingi protsendi võrra väiksem, siis valib uue palli jahtimiseks.
//KAHE SARNASE PALLI VAHEL VALIMINE
					//std::cout<<"kauguste vahe"<<std::endl;
					//std::cout<<abs(maxObj.distanceH) - abs(obj->distanceH)<<std::endl;
							if((obj->area < maxObj.area && obj->area > 0.7*maxObj.area) && (abs(maxObj.distanceH) - abs(obj->distanceH) > 60 )&&maxObj.area > 300)
								{ 
								maxObj = *obj;
								}
						}//kui type=BALL l6pp
						if(maxObj.type != -1 && !pallN2ha) 
							{
							pallN2ha = true;
							palliPyydmisTimer.restart();
							}
					//KUI PALL ON PAREMAL
						if(pallN2ha)
							{
							if(maxObj.distanceH >= 25) 
								{
								if(maxObj.area < 2500)
									{
									palliAsukoht = PALL_PAREMAL;
									} 
								else 
									{ 
									palliAsukoht = PALL_L2HEDAL_P;
									}
								}

				//KUI PALL ON VASAKUL
								if(maxObj.distanceH <= -25) 
									{
									if(maxObj.area<2500)
										{		
										palliAsukoht = PALL_VASAKUL;
										} 
									else 
										{
										palliAsukoht = PALL_L2HEDAL_V;
										}
									}
				//KUI PALL ON OTSE
								if(maxObj.distanceH > -30 && maxObj.distanceH < 30) 
									{ 
									palliAsukoht = PALL_OTSE;
									}
								if(maxObj.rect.y > 130 && abs(maxObj.distanceH) < 50) 
									{
									palliAsukoht = PALL_L2HEDAL_OTSE;
									}
						} //if maxobj.type != -1 l6pp
						if(maxObj.type == -1) 
							{
							pallN2ha = false;
							}			
						
				}//if !pallK2es l6pp
					if(pallK2es) {
						if(obj->type==v2ravaTyyp) 
							{
								if(obj->rect.width > v2rav.rect.width) 
								{
								v2rav = *obj;
								}
							}
						if(v2rav.type != -1) 
							{
							v2ravN2ha = true;
							/*pidur();
							msleep(400);
							clearBuffer();
							getAnalogs();*/
							algkiirusKP = MIN_KIIRUS_KP;
							}

		//KUI V2RAV ON PAREMAL
						if(v2ravN2ha && pallK2es) 
							{
							if((v2rav.distanceH > 30 && v2rav.rect.width > 75) || (v2rav.distanceH > 18 && v2rav.rect.width <= 75)) 
								{
								v2ravaAsukoht = V2RAV_PAREMAL;
								}
		//KUI V2RAV ON VASAKUL
							if((v2rav.distanceH < -30 && v2rav.rect.width > 75) 
								|| (v2rav.distanceH < -18 && v2rav.rect.width <= 75)) 
								{	
								v2ravaAsukoht = V2RAV_VASAKUL;		
								}


		//KUI V2RAV ON OTSE
							if((v2rav.rect.width > 75 && (abs(v2rav.distanceH)<=30)) || 
								(v2rav.rect.width <=75 && (abs(v2rav.distanceH)<=18))) 
								{
								v2ravaAsukoht = V2RAV_OTSE;
								}

							/*if(v2rav.rect.width >= 130 && abs(v2rav.distanceH)>50) 
										{	
										v2ravaLaius = V2RAV_LAI;
										}
									if((v2rav.rect.width >= 70 && v2rav.rect.width < 130) || (abs(v2rav.distanceH)<50 && v2rav.rect.width >= 130))
										{
										v2ravaLaius = V2RAV_KESKMINE;
										}
									if(v2rav.rect.width < 70) 
										{
										v2ravaLaius = V2RAV_KITSAS;
										}
*/
							} //if v2ravn2ha && pall2es l6pp
							
							
						}// if pallK2es l6pp


						
					//debugimiseks
        			//std::cout<<((obj->type))<<std::endl;
				//std::cout<<((obj->distanceH))<<std::endl;
        			//std::cout<<((obj->area))<<std::endl;

      				}//if l6pp
    

  			 }//for l6pp
	} //if l6pp
					//std::cout<<((v2rav.area))<<std::endl;
					//std::cout<<((maxObj.rect.y))<<std::endl;
					//std::cout<<"pindala suuremal"<<std::endl;
			
							if(v2rav.type == -1) 
								{
								v2ravN2ha = false;
								}  

	getAnalogs();

	if(analog[5]>400 && !pallK2es) {  //kui palli ei ole pesas, n2itab testide j2rgi (seisuga 1. dets) pallipesaandur v22rtust ca 80-120
		pallK2es = true;
		if(!palliOtsimisTimerT88tab)
			{
			palliPyydmisTimer.start();
			palliOtsimisTimerT88tab = true;
			}
		palliPesaTimer.restart();
		} 
	if (analog[5] <= 400 && pallK2es) {
		pallK2es = false; 
		}
	if((analog[0] > 110 || analog[1] > 220) && (analog[0] < 250 || analog[1] < 280)) {
		seinaKaugus = SEIN_L2HEDAL; // vt kas kusagil yldse kasutatakse
		} 
	if (analog[0] <= 110 || analog[1] <= 220)
		{
		seinaKaugus = SEIN_KAUGEL;
		seinaTimer.restart();
		}
	if (analog[0] >= 250 || analog[1] >= 280)
		{
		seinaKaugus = SEIN_K6RVAL;
		if(seinaTimer.elapsed() > 1500 && !pallN2ha && !pallK2es)
			{
			seinaTimer.restart();
			eemaldu();
			}
		}
	if((analog[2] > 100 || analog[3] > 100) && ((analog[2] < 350 || analog[3] < 350)) && seinaKaugus == SEIN_KAUGEL) 
		{
		v2ravaKaugus = V2RAV_L2HEDAL;
		} 
	if(analog[2]<= 100 || analog[3] <= 100)
		{
		v2ravaKaugus = V2RAV_KAUGEL;
		}
	if((analog[2] >= 350 || analog[3] >= 350) && seinaKaugus == SEIN_KAUGEL)
		{
		v2ravaKaugus = V2RAV_K6RVAL;
		}



	/*if(analog[7] < 700 && analog[6] < 700)
		{
		v2ravaSignaal = V2RAV_OTSE;
		}
	/*if(analog[7] > 900 && analog[6] > 900)
		{
		v2ravaSignaal = V2RAV_TAGA;
		}*/
	
//SEINAST JA ROBOTIST EEMALEHOIDMINE OTSIMISE AJAL

		if(!pallK2es && pallN2ha) { //kui palli pole k2es, aga pall on n2ha

			if(seinaKaugus == SEIN_K6RVAL) { //kui sein ligineb			
				otseKiirus = otseKiirusL2hedal;
			
			} else {
				otseKiirus = MAX_KIIRUS;
			}
		}

		if(!pallN2ha && !pallK2es && !pallL2hedal) { //kui palli ei ole n2ha ja pall pole k2es 
	//TODO sleep'ga midagi ette võtta - otsimine ymber teha tingimuseks - if (pallK2es && andurid ei n2ita seina)
	//getAnalogs();
		if(seinaKaugus == SEIN_K6RVAL) 
			{	
			//std::cout<<"KPp88reP"<<std::endl;
			//std::cout<<analog[0]<<" vasak"<<std::endl;
			//std::cout<<analog[1]<<" parem"<<std::endl;
			//eemaldu();
			tagurda();
			msleep(100);
			p88ra(255, 510);
			msleep(150);
			clearBuffer();
			getAnalogs();			
			}
		}

		if(pallK2es) 
			{
			if(!v2ravN2ha) 
				{
				otsiV2rav();
				}
			if(v2ravN2ha)
				{
				//kui v2rav on paremal

				if(v2ravaAsukoht == V2RAV_PAREMAL) 
					{
					p88reKeskV2ravaleP();
					/*if(v2ravaLaius = V2RAV_LAI) 
						{	
						p88reL2hiV2ravaleP();
						}
					if(v2ravaLaius = V2RAV_KESKMINE)
						{
						p88reKeskV2ravaleP();
						}
					if(v2ravaLaius = V2RAV_KITSAS) 
						{
						p88reKaugV2ravaleP();
						}
					*/
					}
			 
			//kui v2rav on vasakul
				if(v2ravaAsukoht == V2RAV_VASAKUL) 
					{	
					p88reKeskV2ravaleV();
					/*if(v2ravaLaius = V2RAV_LAI) 
						{	
						p88reL2hiV2ravaleV();
						}
					if(v2ravaLaius = V2RAV_KESKMINE)
						{
						p88reKeskV2ravaleV();
						}
					if(v2ravaLaius = V2RAV_KITSAS) 
						{
						p88reKaugV2ravaleV();
						}
					*/
					}

			//kui värava pindala on suht suur, siis võib distanceH olla suurem ka - TODO!
				if(v2ravaAsukoht == V2RAV_OTSE) 
					{
					//std::cout<<"v2rav otse"<<std::endl;
					if(v2rav.rect.width < 30)
						{
						otse();
						}
					else 
						{
						if (!sihibKeskeleTimerT88tab)
							{
							sihibKeskeleTimer.restart();
							sihibKeskeleTimerT88tab = true;
							}
							std::cout<<v2rav.distanceH<<std::endl;
							std::cout<<v2rav.rect.width<<std::endl;
						if (sihibKeskeleTimer.elapsed() < 250)
							{
							pidur();
							std::cout<<"sihin"<<std::endl;
							}
						if (palliPesaTimer.elapsed() > 500 && sihibKeskeleTimer.elapsed()>150) 
							{
							kickBall();
							std::cout<<"l6in palli"<<std::endl;
							}

						}
					}
				}//if v2ravN2ha l6pp
			} //if pallK2es l6pp



//KUI PALL EI OLE KÄES
	if(!pallK2es) {

	if(analog[7] < 700 && analog[6] > 800) 
		{
		v2ravaSignaal = V2RAV_VASAKUL;
		//std::cout<<"v2ravaSignaal = vasakul"<<std::endl;
		}
	if(analog[6] < 700 && analog[7] > 900)
		{
		v2ravaSignaal = V2RAV_PAREMAL;
		//std::cout<<"v2ravaSignaal = paremal"<<std::endl;
		}

//PALLI OTSIMINE - toimub juhul, kui palli pole n2ha ja pall pole k2es

		if(!pallN2ha && !pallL2hedal) 
		{
	//	KPp88reP();
		otsiPall();
		}
//TODO - kui pall on kaugemal, siis v6iks pallipyydmistimer pikemalt olla ja vastupidi
		if(pallN2ha)
			{
			if(palliPyydmisTimer.elapsed() > 5000 && !pallK2es)
				{
				eemaldu();	
				}
	//	KPp88reP();
		
	//PALLI POOLE LIIKUMINE

			if(!pallL2hedal) //kui ei toimu l2hipallile s6itmine
				{
	
		//kui pall on paremal
				if(palliAsukoht == PALL_PAREMAL) 
					{
					p88reKaugPallileP();
					} 
				if(palliAsukoht == PALL_L2HEDAL_P)
					{
					p88reL2hiPallileP();
					}
		 
		//kui pall on vasakul
				if(palliAsukoht == PALL_VASAKUL) 
					{
					p88reKaugPallileV();
					}
				if(palliAsukoht == PALL_L2HEDAL_V)
					{		
					p88reL2hiPallileV();
					} 

		//kui pall on otse

				if(palliAsukoht == PALL_OTSE) 
					{ 
					otse();
					}
				}//!pallL2hedal l6pp

			
			//kui pall on nii l2hedal, et j22b kohe kaamerapildist v2lja
				if(palliAsukoht == PALL_L2HEDAL_OTSE && !pallL2hedal) 
					{
					pallL2hedal = true;
					pallL2hedalTimer.restart();
					}

				if(pallL2hedal) 
					{
					//std::cout<<" pallL2hedal state"<<std::endl;
					otseL2hiPallile();
					}

		} // if pallN2ha l6pp


		

//V2RAVAST EEMALEHOIDMINE
//robot võib ka olla v2rava k6rgune - ei saa p6geneda teise roboti eest /st et kui alumised andurid ei n2ita, aga ylemised n2itavad, siis on v2rav
	
	if(!pallL2hedal) 
		{
		if(v2ravaKaugus == V2RAV_K6RVAL && pallN2ha) 
			{
			eemaldu();
			}
		}

//TODO - kui pall ei ole k2es ja palli ei ole n2ha, peaks v2ravast varem 2ra p88rama

		/*if(v2ravL2hedal && !pallN2ha) 
			{
			KPp88reP();
			msleep(300);
			//std::cout<<"KPp88reP ja v2ravL2hedal && !pallN2ha"<<std::endl;
			clearBuffer();
			getAnalogs();
			}*/
		}
	
//TODO - kaunter, et ei teeks reopen't liiga tihti - nt 2 sekundit
	if(image->fps < 8 && pildiReopenTimer.elapsed() > 2000)
		{
 	    image->reOpen();
 	    qDebug("FPS is too slow...reopening Camera!!!");
		pildiReopenTimer.restart();
		}


		pildiKaunter++;
	
		if(pildiKaunter==4) 
			{
 			view->show(_img);
			pildiKaunter = 0;
			}



	}//while l6pp


	///TESTID
	///Testid while(0) õige programmi ajal. Testimise ajaks while(1). 
//TEST OTSES6IDUKS MAX KIIRUSEL JA SEINA JUURES SEISMAJ22MINE
	while(0) 
		{
		p88ra(240, 255);
		getAnalogs();
		while(analog[0] > 200 || analog[1] > 200) 
		{
		std::cout<<"seisan"<<std::endl;
		pidur();
		getAnalogs();
		}
	}
//KIIRUSE T&STMINE P88RAMISEL - MIS KIIRUSE JUURES PALL PYSIB PESAS
//PYSIB PESAS MAX KIIRUSE JUURES, KUI KIIRENDUS ON MAX V2HEMALT 15 100MS KOHTA. SOOVITATAV KASUTADA 10. 			if(kiirusevahe>10) {

//otsimine.start();
int i = 60;

while(0) {
	getAnalogs();
	while(analog[5]>800) { 
		p88ra(i, 255+i);
		if(otsimine.elapsed() > 100)
			{	
			if(i+60>246) {
				p88ra(255, 511);
				sleep(4);
				p88ra(511, 255);
				sleep(4);
				break;
				}	
				i+=10;
			std::cout<<"liitsin i" <<i<<std::endl;
				otsimine.restart();
			}
getAnalogs();

		}
	std::cout<<"kiirus "<<i<<std::endl;
pidur();
break;
}

// Seriali kiiruse test
    // Loeb korraga kõiki analooge
while(1){
    //gettimeofday(&a, NULL);
    getAnalogs();

    std::cout<<"---------------------------"<<std::endl;	
    std::cout<<analog[0]<<" on vasak sein"<<std::endl; // eesmine vasak andur
    std::cout<<analog[1]<<" on parem sein"<<std::endl; // eesmine parem andur
   // std::cout<<"v2rav parem"<<std::endl; // majaka vasak*/
    std::cout<<analog[2]<<" on parem v2rav"<<std::endl; // v2rav parem andur
    std::cout<<analog[3]<<" on vasak v2rav"<<std::endl; // v2rav vasak andur
    //std::cout<<analog[4]<<std::endl; */
    //std::cout<<"palli pesa: "<<std::endl;//u 600 on anduri n2it, kui palli ei ole pesas
    //std::cout<<analog[5]<<std::endl; // palli pesa*/
    std::cout<<analog[6]<<std::endl; // majaka parem
	std::cout<<" on majaka parem"<<std::endl;
    std::cout<<analog[7]<<std::endl; // majaka vasak
	std::cout<<" on majaka vasak"<<std::endl;
    //gettimeofday(&b, NULL);
    //std::cout<<"Time: "<<b.tv_usec-a.tv_usec<<std::endl;
    sleep(1);
}

getAnalogs();
std::cout<<analog[0]<<std::endl; // eesmine vasak andur
std::cout<<analog[1]<<std::endl; // eesmine parem andur
std::cout<<analog[2]<<std::endl; // v2rav parem andur
std::cout<<analog[3]<<std::endl; // v2rav vasak andur
std::cout<<analog[4]<<std::endl; //
std::cout<<analog[5]<<std::endl; // palli pesa
std::cout<<analog[6]<<std::endl; // majaka vasak
std::cout<<analog[7]<<std::endl; // majaka parem

setDcMotor(2, 512);


while(0){ //v2rava anduri (parema anduriga) löögitest
   getAnalogs();
   if (analog[2] > 200) kickBall();
}

//Pall pesas, siis löök
while(0) {
	getAnalogs();
	if (analog[2] >= 100) kickBall();
}

// otses6idu test
while(0){
	otse();
	sleep(3);
	pidur();
	sleep(10);
}



// P66RAMINE
//ruut otse ja vasakule pöörates
while(0){
	otse();
	sleep(2.5);
	pidur();
	sleep(1);
	p88ra(510, 255);
	sleep(2.5);
	pidur();
	sleep(1);

//tagasi ja paremale
	liigu(510);
	sleep(2.5);
	pidur();
	sleep(1);
	p88ra(255, 510);
	sleep(2.5);
	pidur();
	sleep(1);
}

//ruut tagasi ja paremale pöörates

while(0){
	liigu(356);
	sleep(2.5);
	pidur();
	sleep(1);
	p88ra(100, 0);
	sleep(2.5);
	pidur();
	sleep(1);
}



//solenoidi test
while(0) {
	setDigital(2);
	msleep(500);
	clearDigital(2);
	msleep(4000);

}

//kickBall();





}


