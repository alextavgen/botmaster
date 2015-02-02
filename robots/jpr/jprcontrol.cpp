/** siin failis on roboti juhtimise funktsioonid */
#include "jprcontrol.h"

const float COS30DEG = (cos(PI / 6.0));

JprControl::JprControl(AbstractRobot* a) {
	abr = a;
	solTime.start();
}

JprControl::~JprControl() {
	delete abr;
}

/** vana omni funktsioon, mida kasutatakse praegu sõitmiseks
funktsiooni puuduseks on see et max kiirus otsesõidul on 255 asemel umbes 220 */
void JprControl::omni(float dirDeg, float velocityBody, int velocityMax, float velocityAngular) {

    omniX(velocityBody, dirDeg, velocityAngular, JPR_MIN_PWM);
//    float velocityX, velocityY;
//	float velocityWheel[3];
//	float max = 0.0;
//	float scaleCoef;
//	float dirRad;
//	int i;
//	int direction;


//	dirRad = dirDeg * 0.0174532925;	// degrees to radians

//	velocityX = velocityBody * cos(dirRad);
//	velocityY = velocityBody * sin(dirRad);

//	//Vw[0] = round(cos(w1) * Vx + sin(w1) * Vy + L * Va);
//	//Vw[1] = round(cos(w2) * Vx + sin(w2) * Vy + L * Va);
//	//Vw[2] = round(cos(w3) * Vx + sin(w3) * Vy + L * Va);

//	velocityWheel[0] = -COS30DEG * velocityX + 0.5 * velocityY + velocityAngular;
//	velocityWheel[1] = COS30DEG * velocityX + 0.5 * velocityY + velocityAngular;
//	velocityWheel[2] = -velocityY + velocityAngular;

//	// Voltage is not linearly proposional to power
//	max = 0.0;
//	for(i = 0; i < 3; i++) {
//		if (abs(velocityWheel[i]) > max) max = abs(velocityWheel[i]);
//	}
	
//	if (max > 0) {
//		for(i = 0; i < 3; i++) {
//			direction = 1;
//			if (velocityWheel[i] < 0) direction = -1;
//			velocityWheel[i] = sqrt((max*max) * (abs(velocityWheel[i]) / max)) * direction;
//		}
//	}

//	// Scaling
//	max = 0.0;
//	for(i = 0; i < 3; i++) {
//                if (abs(velocityWheel[i]) > max) max = abs(velocityWheel[i]);
//	}
	
//	if (max > velocityMax) {
//		scaleCoef = velocityMax / max;
//		for(i = 0; i < 3; i++) {
//			velocityWheel[i] = velocityWheel[i] * scaleCoef;
//		}
//	}
//        for(int i=0;i<3;i++){
//            pwm[i]=round(velocityWheel[i]);
//            abr->setDcMotor(i, pwm[i]);
//        }
}



/** paneb kõik sõidumootorid seisma*/
void JprControl::omniBreak() {
        for(int i=0;i<3;i++){
            pwm[i]=512;
            abr->setDcMotor(i, pwm[i]);
        }
}

/** solenoidi löök, löökide vaheline min aeg 1 sekund
TODO: teha usleep väiksemaks */
void JprControl::solenoid(bool s) {
    if(solTime.elapsed()>1000){
        // võib lüüa mitte tihedamalt kui 1 sekund
        abr->setDigital(0);
        solTime.restart();
        if(s){
            usleep(100 * 1000);
        }
    }
}

// solenoidi väljalülitamine, vb teha hoopis QT signaal
void JprControl::checkSolenoid() {
    if(solTime.elapsed()>100){
        // ilma sleepita variant, solenoidi väljalülitamiseks
        abr->clearDigital(0);
    }
}

/** saadab mootoritele kiirused mis ta uues omni fn-is välja arvutab? */
void JprControl::sendWheelSpeeds(){
    for(int i=0;i<3;i++){
        pwm[i]=round(wsi(i));
        abr->setDcMotor(i, pwm[i]);
    }
}

/** valgustuse lülitamine */
void JprControl::headLight(bool on){
	if(on){
		abr->setDigital(1);
	} else {
		abr->clearDigital(1);
	}
}

/** rulli lülitamine */
void JprControl::reel(bool on){
	if(on){
		abr->setDcMotor(3,-255);
	} else {
		abr->setDcMotor(3, 0);
	}
}

