#include "halmath.h"

HalMath::HalMath()
{
}

void HalMath::omni( int pwms[], float dirDeg, float velocityBody, int velocityMax, float velocityAngular, float battery) {
    float velocityX, velocityY;
    float velocityWheel[3];
    float max = 0.0;
    float scaleCoef;
    float dirRad;
    int pwm[3];
    int pwmSum = 0;
    int direction;
    int i;

    dirRad = dirDeg * 0.0174532925; // degrees to radians

    // Correcting speed according battery
    direction = 1;
    if (velocityBody < 0) direction = -1;
    velocityBody = sqrt((velocityBody * velocityBody) * battery) * direction;

    direction = 1;
    if (velocityAngular < 0) direction = -1;
    velocityAngular = sqrt((velocityAngular * velocityAngular) * battery) * direction;


    velocityX = velocityBody * cos(dirRad);
    velocityY = velocityBody * sin(dirRad);

    //Vw[0] = round(cos(w1) * Vx + sin(w1) * Vy + L * Va);
    //Vw[1] = round(cos(w2) * Vx + sin(w2) * Vy + L * Va);
    //Vw[2] = round(cos(w3) * Vx + sin(w3) * Vy + L * Va);

    // 150, 30, 270 (60, 300, 180)
    velocityWheel[0] = -0.866025404 * velocityX + 0.5 * velocityY + velocityAngular;
    velocityWheel[2] = 0.866025404 * velocityX + 0.5 * velocityY + velocityAngular;
    velocityWheel[1] = -velocityY + velocityAngular;

    // Voltage is not linearly proposional to power
    max = 0.0;
    for (i = 0; i < 3; i++) {
        if (abs(velocityWheel[i]) > max) max = abs(velocityWheel[i]);
    }

    if (max > 0) {
        for (i = 0; i < 3; i++) {
            direction = 1;
            if (velocityWheel[i] < 0) direction = -1;
            velocityWheel[i] = sqrt((max * max) * (abs(velocityWheel[i]) / max)) * direction;
        }
    }

    // Scaling
    max = 0.0;
    for (i = 0; i < 3; i++) {
        if (abs(velocityWheel[i]) > velocityMax) max = abs(velocityWheel[i]);
    }

    if (max > velocityMax) {
        scaleCoef = velocityMax / max;
        for (i = 0; i < 3; i++) {
            velocityWheel[i] = velocityWheel[i] * scaleCoef;
        }
    }

    for (i = 0; i < 3; i++) {
        pwm[i] = round(velocityWheel[i]);
        pwmSum = pwmSum + pwm[i];
    }

    for (i = 0; i < 3; i++) {
        // If not all wheels are stopped break wheels with 0 speed
        if (pwm[i] == 0 && pwmSum != 0) {
            //setDcMotor(i, 512);
            // set the PWM value
            pwms[i] = 512;
        } else {
            //setDcMotor(i, pwm[i]);
            // set the PWM value
            pwms[i] = pwm[i];
        }
    }
}


//Arvutame palli kauguse ~(cm)
int HalMath::palliKaugus(int y)
{
	int kaugus = (int) (308362.484895514 * pow(y, -2.16049138430261));
	//int kaugus = (int) (342.781116297389 * pow(y, -0.448966183073739));
    //int kaugus = (int)(209.460992688557 * pow(y,-0.980263301970066) * 10 - 10);
    //int kaugus = (int) (3524.1113825362 * pow(y, -1.2495304685));
    //int kaugus = (int) (145.454390312423 * pow(0.976310673281194, y));
    return kaugus;
}

//TODO: Arvutame värava kauguse
int HalMath::varavakaugus( int y )
{
    int kaugus = y;
    return kaugus;
}
