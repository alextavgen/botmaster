 
       //
// C++ Interface: tolmurull
//
// Description: 
//
//
// Author: Valdur Kaldvee, Margus Ernits <margus.ernits@itcollege.ee>, Katrin Kibbal (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PELMEEN_H
#define PELMEEN_H

#include "../../abstractrobot.h"
//#include "objectfinder.h"
#include "image.h"
#include "view.h"
#include <iostream>
#define CAN  1
#define SOCK 2
#define CAN_BEACON 3
#define SOCK_BEACON 4

/**
	@author  Margus Ernits <margus.ernits@itcollege.ee>
*/



class Pelmeen: public AbstractRobot{
public:
  
    Pelmeen();

    ~Pelmeen();
    
    void go();

int i, suund, kiirus, otsing;
int sein_vasakul();
int sein_paremal();
void viivitus(long t);

void lmotor_speed(int speed)
{
setServo(1,118-speed);
}
void rmotor_speed(int speed)
{
setServo(2,118+speed);
}



inline void tagasi(int speed)
{
	lmotor_speed(speed*(-1)); //39
	rmotor_speed(speed*(-1)); //39
}


//-----------------------------------


inline void edasi_kiiresti()
{ //otse kiiresti
	lmotor_speed(39); //39
	rmotor_speed(36); //23
}
inline void tagasi_kiiresti()
{ //otse kiiresti
	lmotor_speed(-39); //39
	rmotor_speed(-36); //23
}

inline void edasi(int kiirus) //otse
{
	if (kiirus < 8)
	{
		lmotor_speed(kiirus+1);
		rmotor_speed(kiirus);
	}
	else if (kiirus >= 8 && kiirus <= 20)
	{
		lmotor_speed(kiirus+2);
		rmotor_speed(kiirus);
	}
	else if (kiirus > 20)
	{
		lmotor_speed(kiirus+3);
		rmotor_speed(kiirus);
	}
}


//-----------------------------------


inline void otsi_paremale()
{
	kohapeal_paremale(4);
}
inline void otsi_vasakule()
{
	kohapeal_vasakule(4);
}


//-----------------------------------


inline void kohapeal_vasakule(int speed)
{
	lmotor_speed((-1)*speed+1); //ei keerle pC'¤ris kohapeal vaid liigume natuke edasi
	rmotor_speed(speed);
}
inline void kohapeal_paremale(int speed)
{
	lmotor_speed(speed);
	rmotor_speed((-1)*speed+1); //vajalik purgi lC'¤hedal olles mitte toppama jC'¤C'¤miseks
}

//-----------------------------------


inline void vasakule_edasi(int speed)
{
	lmotor_speed(0);//39
	rmotor_speed(speed);
}
inline void paremale_edasi(int speed)
{
	lmotor_speed(speed);
	rmotor_speed(0);//39
}

inline void sokkyles()
{
	int juust = 0;
	while (getAnalog(4) > 70)
			{
				setServo(5,255);
				edasi(7);
				juust++;
				if (juust > 20)
				{
					tagasi(3);
					viivitus(1000);
					juust = 0;
				}
				std::cout<<"imeb sokki"<<std::endl;
			}
	setServo(5,125);
}


//-----------------------------------


inline void vasakule_tagasi(int speed)
{
	lmotor_speed((-1)*speed);//39
	rmotor_speed(0);
}
inline void paremale_tagasi(int speed)
{
	lmotor_speed(0);
	rmotor_speed((-1)*speed);//39
}


//-----------------------------------


inline void aeglaselt_vasakule()
{
	lmotor_speed(8);
	rmotor_speed(11);
}
inline void aeglaselt_paremale()
{
	lmotor_speed(11);
	rmotor_speed(7);
}

//------------------------------------


inline void kiiremini_vasakule()
{
	lmotor_speed(2);
	rmotor_speed(5);
}

inline void kiiremini_paremale()
{
	lmotor_speed(6);
	rmotor_speed(2);
}

//-----------------------------------


inline void k2pp()
{
	stop();
	setServo(0,50);
	viivitus(750);
	setServo(0,172);
	viivitus(400);
	setServo(3,85);
	viivitus(900);
	setServo(3,185);
	viivitus(200);
	setServo(0,90);
}
inline void kast()
{
	setServo(4,156);
	viivitus(1500);
	setServo(4,92);
	viivitus(600);
	setServo(4,156);
	viivitus(700);
	setServo(4,92);
}

//-----------------------------------

inline void otsi_majakat()
{
	if (otsing == 0)		//otsing 0 vasakule (vastupäeva)
	{
		lmotor_speed(-3);
		rmotor_speed(3);
	}
	else if (otsing == 1)		//otsing 1 paremale (päripäeva)
	{
		lmotor_speed(3);
		rmotor_speed(-3);
	}
}
inline void sokidmaha()
{
	edasi_kiiresti();
	viivitus(6000);
	setServo(5,125);
}
inline void purgidmaha()
{
	if (getAnalog(2) > 200 && (getAnalog(2) > getAnalog(3)))
	{
		// std::cout<<getAnalog(2)<<"parem"<<std::endl;
		// std::cout<<getAnalog(3)<<"vasak"<<std::endl;
		// std::cout<<"tuleb paremalt"<<std::endl;
		paremale_edasi(10);
		viivitus(400);
		paremale_tagasi(20);
		viivitus(1600);
		tagasi(3);
		viivitus(1000);
		stop();
		kast();
		paremale_edasi(8);
		viivitus(500);
	}
	else if (getAnalog(3) > 200 && (getAnalog(3) > getAnalog(2)))
	{
		// std::cout<<getAnalog(2)<<"parem"<<std::endl;
		// std::cout<<getAnalog(3)<<"vasak"<<std::endl;
		// std::cout<<"tuleb vasakult"<<std::endl;
		vasakule_edasi(10);
		viivitus(300);
		tagasi(20);
		viivitus(800);
		paremale_edasi(20);
		viivitus(1400);
		edasi(20);
		viivitus(400);
		stop();
		kast();
		paremale_edasi(8);
		viivitus(500);
	}
	else
	{
		// std::cout<<getAnalog(2)<<"parem"<<std::endl;
		// std::cout<<getAnalog(3)<<"vasak"<<std::endl;
		// std::cout<<"tuleb keskelt"<<std::endl;
		edasi(5);
		viivitus(300);
		kohapeal_paremale(10);
		viivitus(1000);
		stop();
		kast();
		paremale_edasi(10);
		viivitus(500);
	}
}


inline void stop()
{
	lmotor_speed(0);
	rmotor_speed(0);
}


};

#endif
