/** uus omni control, keskpunkti nihutamine alli keskpunktile et kergemini pallile sõita*/
#include "jpromnicontrol.h"

#include <math.h>
#include <iostream>

using namespace std;
/** konstruktor, pannakse paika rataste nurgad põhja plaadi keskpunkti suhtes (0 ees)*/
JprOmniControl::JprOmniControl()
{
    calcCenterPoint = QVector2D(0,0);
    // nurgad x teljest, ja tagumine ratas on x telje negatiivses osas
    degW[2] = 180;
    // esimene eestvaates vasakpoolne
    degW[1] = -60;//300;
    // esimene eestvaates parempoolne
    degW[0] = 60;

    for(int i=0;i<3;i++){
        // kaugusühikvektorid
        vectWD[i] = QVector2D(cos(rad(degW[i])), sin(rad(degW[i])));
        // jõuühikvektorid
        vectWF[i] = QVector2D(-sin(rad(degW[i])),cos(rad(degW[i])));
    }

}

/** arvutab ratta kaaalud, et teaks mis on rataste suhtelised kiirused*/
void JprOmniControl::omniVect(double vx, double vy, double w){
    //velocity.normalize();   // vektori pikkuseks on üks

    // rataste paigutuse maatrix
    double dataF[3][3];
	double degr=0;
    for(int i=0;i<3;i++){
		degr = angleBetween(centerVector(i),QVector2D(1,0));
		if(centerVector(i).y()<0)degr = -degr;
		vectWF[i] = QVector2D(-sin(rad(degr)),cos(rad(degr)));      
		dataF[i][0] = vectWF[i].x();
        dataF[i][1] = vectWF[i].y();
        dataF[i][2] = centerDistanceW(i);
        //cout<<"d"<<i<<": "<<angleBetween(vectWD[i],centerVector(i))<<endl;
        //cout<<"d"<<i<<": "<<angleBetween(centerVector(i),QVector2D(1,0))<<endl;
    }
    QGenericMatrix<3,3,double> forceMat((double *)&dataF);

    // soovitud keha kiirus ja nurk
    QGenericMatrix<1,3,double> bodyVelocityMatrix;
    qreal *dataW = bodyVelocityMatrix.data();
    dataW[0] = vx;  // kiiruse x komponent
    dataW[1] = vy;  // kiiruse y komponent
    dataW[2] = w;   // nurkkiirus

    // iga üksiku ratta kiirus
    QGenericMatrix<1,3,double> wheelSpeedsMat = forceMat * bodyVelocityMatrix;
    for(int i=0;i<3;i++){
        wheelSpeeds[i] = wheelSpeedsMat.data()[0,i];
    }

    // skaleerib suurima väärtuse numbrini üks
    // scaleWheelSpeeds(scale);
}

/** skaleerib mootorite kiirused vahemikku -1 .. 1 */
void JprOmniControl::scaleWheelSpeeds(){
    // suurem väärtus skaleerida üheks, sama kordajaga kasvatada/kahandada ka teisi
    scaleWheelSpeeds(1);
}

/** üks segane fn millega midagi prooviti, aga töötab nii nagu vanas omnis*/
void JprOmniControl::scalePower(){
    // Voltage is not linearly proportional to power
    double *data = wheelSpeeds;
    double max=0;
    double dat=0;
    for(int i=0;i<3;i++){
        dat = abs(data[i]);
        if(dat>max)max = dat;
    }
    int direction = 1;
    if (max > 0) {
        for(int i = 0; i < 3; i++) {
            direction = 1;
            if (data[i] < 0) direction = -1;
            data[i] = sqrt(max * abs(data[i])) * direction;
        }
    }
}

/** skaleerib mootorite kiirused vahemikku -n .. n */
void JprOmniControl::scaleWheelSpeeds(int n){
    // suurem väärtus skaleerida üheks, sama kordajaga kasvatada/kahandada ka teisi
    if(n!=0){
        double *data = wheelSpeeds;
        double max=0;
        double dat=0;
        for(int i=0;i<3;i++){
            dat = abs(data[i]);
            if(dat>max)max = dat;
        }
        if(max <= 0)max = 1;
        max /=abs(n);
        for(int i=0;i<3;i++){
            data[i]/=max;
        }
    }
}

/** veel mingi vana mootorite kiiruste skaleerimine  */
void JprOmniControl::scaleWheelSpeedsOld(int n){
    // vana omni jaoks
    if(n!=0){
        double *data = wheelSpeeds;
        double max=0;
        double dat=0;
        for(int i=0;i<3;i++){
            dat = abs(data[i]);
            if(dat>max)max = dat;
        }
        if (max>abs(n)){
            if(max <= 0)max = 1;
            max /=abs(n);
            for(int i=0;i<3;i++){
                data[i]/=max;
            }
        }
    }
}

void JprOmniControl::scaleToMinSpeed(double s){
    // s = näiteks 65/256
    double *data = wheelSpeeds;
    double d = 0;
    for(int i=0;i<3;i++){
        d = data[i];
        data[i] = 1-((1-abs(d))*(1-s));
        // märk tagasi õigeks
        if(d<0)data[i] = -data[i];
        // nulli jätame nulliks
        if(abs(d)<0.00001)data[i] = 0;
    }
}

/** vana omni fn*/
void JprOmniControl::omniVana(float dirDeg, float velocityBody, int velocityMax, float velocityAngular){
    float velocityX, velocityY;
    float velocityWheel[3];
    float max = 0.0;
    float scaleCoef;
    float dirRad;
    int i;
    int direction;
    const float COS30DEG = (cos(PI / 6.0));

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
        if (abs(velocityWheel[i]) > max) max = abs(velocityWheel[i]);
    }

    if (max > velocityMax) {
        scaleCoef = velocityMax / max;
        for(i = 0; i < 3; i++) {
            velocityWheel[i] = velocityWheel[i] * scaleCoef;
        }
    }

    double *data = wheelSpeeds;
    data[0] = round(velocityWheel[0]);
    data[1] = round(velocityWheel[1]);
    data[2] = round(velocityWheel[2]);

    sendWheelSpeeds();
}

/** katsetamiseks tehtud, mingi ühilduvuse värk*/
void JprOmniControl::omniUuegaVana(float dirDeg, float velocityBody, int velocityMax, float velocityAngular){
    double x = velocityBody*cos(rad(dirDeg));
    double y = velocityBody*sin(rad(dirDeg));
    omniVect(x, y, velocityAngular);
    scaleWheelSpeedsOld(velocityMax);
    scalePower();
    sendWheelSpeeds();
}

/** loll katsetus, keskpunkti nihutamiseks */
void JprOmniControl::setCalcCenterDegree(double len, double degree){
    double x = len*cos(rad(degree));
    double y = len*sin(rad(degree));
    setCalcCenterPoint(x,y);
}
/** uus omni, ralli kasutab väravale pööramisel
vx - kiirusvektori x komponent
vy - kiirusvektory y komponent
x - telg on roboti vaatesuunas
*/
void JprOmniControl::omniN(double vx, double vy, double angularVelocity, int maxSpeed, int minSpeed){
    omniVect(vx, vy, angularVelocity);
    turnWheels();
    scaleWheelSpeeds(1);
    scaleToMinSpeed(((float)minSpeed)/float(maxSpeed));
    scaleWheelSpeeds(maxSpeed);
    //scalePower();
    sendWheelSpeeds();
}
/** katsetus */
void JprOmniControl::omniNV(double heading, double velocityBody, int velocityMax, double velocityAngular){
    double x = velocityBody*cos(rad(heading));
    double y = velocityBody*sin(rad(heading));
    omniVect(x, y, velocityAngular);
    if(velocityBody>255)velocityBody=255;
    scaleWheelSpeeds(velocityBody);
    scalePower();
    sendWheelSpeeds();
}

/**

*/
void JprOmniControl::omniX(int velocityBody, double heading, double angularVelocity, int minSpeed){
    double x = velocityBody*cos(rad(heading));
    double y = velocityBody*sin(rad(heading));
    omniVect(x, y, angularVelocity);
    turnWheels();
    scaleWheelSpeeds(1);
    velocityBody = abs(velocityBody);
    angularVelocity = abs(angularVelocity);
    minSpeed = abs(minSpeed);
    if(velocityBody<angularVelocity)velocityBody = angularVelocity;
    if(velocityBody<minSpeed)velocityBody = minSpeed;
    scaleToMinSpeed(((float)minSpeed)/float(velocityBody));
    if(velocityBody>255)velocityBody=255;
    scaleWheelSpeeds(velocityBody);
    //scalePower();
    sendWheelSpeeds();
}

/** nurk kahe vektori vahel */
double JprOmniControl::angleBetween(QVector2D a, QVector2D b){
    return deg(acos(a.dotProduct(a.normalized(),b.normalized())));
}

/** katsetus, keskpunkti nihutamiseks */
void JprOmniControl::turnWheels(){    
	for(int i=0;i<3;i++){        
		if(!(calcCenterPoint.isNull())){
            double d = angleBetween(centerVector(i),QVector2D(1,0));
			if(centerVector(i).y()<0)d = -d;
			wheelSpeeds[i] *= cos(rad(degW[i]-d));
			//cout<<degW[i]<<"x"<<d<<"x"<<endl;
			//if((degW[i]-d)>0)d = 1; else d = -1;
			//cout<<d<<endl;
            //wheelSpeeds[i] *= d;
        }
    }
}
