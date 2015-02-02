#ifndef JPTOPSECRET_H
#define JPTOPSECRET_H
#include <QThread>
#include <iostream>
#include "../../abstractrobot.h"
#include "../../objecttype.h"
#include "../../comm.h"
#include <QTime>


#define XV_ANDUR (jpr->analog[0]);
#define XP_ANDUR (jpr->analog[1]);

#define ANDUR_SEIN 150
#define V_ANDUR ((jpr->analog[0])>150)
#define P_ANDUR ((jpr->analog[1])>150)
#define SEIN_EES (V_ANDUR && P_ANDUR)
#define SEIN_VASAKUL (V_ANDUR && !P_ANDUR)
#define SEIN_PAREMAL (P_ANDUR && !V_ANDUR)
#define SEIN (SEIN_EES || SEIN_VASAKUL || SEIN_PAREMAL)
#define PALL_EES1 ((jpr->analog[2])>900)
#define PALL_EES2 ((jpr->analog[3])>900)
#define PALL_EES (PALL_EES1 || (PALL_EES2 && PALL_EES1))
#define NURK ((jpr->analog[0])>490 && (jpr->analog[1])>490 && (jpr->analog[5]< 200) )
//#define NURK ((jpr->analog[0])>560 && (jpr->analog[1])>460 && (jpr->analog[5]< 200) )

#define IR_V_ANDUR ((jpr->analog[7])<100)
#define IR_P_ANDUR ((jpr->analog[6])<100)
#define VARAV_EES (IR_V_ANDUR && IR_P_ANDUR)
#define VARAV_VASAKUL (IR_V_ANDUR && !IR_P_ANDUR)
#define VARAV_PAREMAL (IR_P_ANDUR && !IR_V_ANDUR)
#define VARAV (VARAV_EES || VARAV_VASAKUL || VARAV_PAREMAL)

//#define YLEMINEV_ANDUR ((jpr->analog[4])>150)
//#define YLEMINEP_ANDUR ((jpr->analog[5])>150)
#define YLEMINEV_ANDUR ((getYlemineV())>100)
#define YLEMINEP_ANDUR ((getYlemineP())>100)
#define VARAV_ANDUR (YLEMINEV_ANDUR || YLEMINEP_ANDUR)

#define NAEB_PALLI (getBall() != NULL)
#define NAGI_PALLI (getBallPrev() != NULL)
#define PALLI_POLE ((getBall() == NULL) && (getBallPrev() == NULL))

#define SV_ANDUR ((jpr->analog[0])>80)
#define SP_ANDUR ((jpr->analog[1])>80)

#define ZERO_SPEED 50

class StatClass{
private:
	int pos;
public:
	StatClass();	
	int arr[20];
	void insert(int nr);
	int avg();	
};

class TopSecretMasterClass{
public:
	//TopSecretMasterClass(Jprobot*);
	//Jprobot* jpr;
	double getYwidth(Image::Object*);

	double laiusToKaugus(int x);			// Y telg ainult
	double laiusToXcm(Image::Object *B);		// X telje suunas

	double getYcm(Image::Object *B);		//arvutab Y-st kauguse
};





#endif
