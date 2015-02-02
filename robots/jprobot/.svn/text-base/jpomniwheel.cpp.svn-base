#include "jpomniwheel.h"

#define MAX_SPEED 255
#define MAX_CANS 6
#define MAX_SOCKS 3
#define MAX_FIND_TIME 150

#define MAX_CAN_CENTERING 5000
#define PI 3.1415926535897932384626433832795

const float COS30DEG = (cos(PI/6.0));

COmni::COmni(Jprobot* v){
	jpr = v;
}

void COmni::omni(float dirDeg, float velocityBody, int velocityMax, float velocityAngular) {
	float velocityX, velocityY;
	float velocityWheel[3];
	float max = 0.0;
	float scaleCoef;
	float dirRad;
	int i;
	int direction;


	dirRad = dirDeg * 0.0174532925;	// degrees to radians

	velocityX = velocityBody * cos(dirRad);
	velocityY = velocityBody * sin(dirRad);

	//Vw[0] = round(cos(w1) * Vx + sin(w1) * Vy + L * Va);
	//Vw[1] = round(cos(w2) * Vx + sin(w2) * Vy + L * Va);	
	//Vw[2] = round(cos(w3) * Vx + sin(w3) * Vy + L * Va);

	velocityWheel[0] = -COS30DEG * velocityX + 0.5 * velocityY + velocityAngular;
	velocityWheel[1] = COS30DEG * velocityX + 0.5 * velocityY + velocityAngular;	
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
		if (abs(velocityWheel[i]) > velocityMax) max = abs(velocityWheel[i]);
	}
	
	if (max > velocityMax) {
		scaleCoef = velocityMax / max;
		for(i = 0; i < 3; i++) {
			velocityWheel[i] = velocityWheel[i] * scaleCoef;
		}
	}

	jpr->setDcMotor(0, round(velocityWheel[0]));
	jpr->setDcMotor(1, round(velocityWheel[1]));
	jpr->setDcMotor(2, round(velocityWheel[2]));
}




void COmni::omniBreak() {
	jpr->setDcMotor(0, 512);
	jpr->setDcMotor(1, 512);
	jpr->setDcMotor(2, 512);
}
