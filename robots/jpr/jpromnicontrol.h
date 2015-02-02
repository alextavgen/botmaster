#ifndef JPROMNICONTROL_H
#define JPROMNICONTROL_H

#include "GeneralDefines.h"
#include <QVector2D>
#include <QGenericMatrix>
#include <cmath>

#include "jprdefines.h"

using namespace std; // selleks et ujukoma abs() töötaks

class JprOmniControl
{
public:
    JprOmniControl();
public:
    double getWheelSpeed(int i){return wheelSpeeds[i];}
    void setCalcCenterPoint(double x, double y){calcCenterPoint = QVector2D(x,y);}
    void setCalcCenterDegree(double len, double degree);
    void omniN(double vx, double vy, double angularVelocity, int maxSpeed, int minSpeed = JPR_MIN_PWM);
    void omniN(double vx, double angularVelocity, int speed){omniN((double)vx,0,angularVelocity,speed);}
private:
    void omniNV(double heading, double velocityBody, int velocityMax, double velocityAngular);
    //void omniN(double heading, double vx, double angularVelocity, int speed);
    void omniV(float heading, float velocityBody, int velocityMax, float velocityAngular){omniUuegaVana(heading, velocityBody, velocityMax, velocityAngular);}    // old signature
    // TODO: kõik muud omniX-d ära kustutada
public:
    void omniX(int velocityBody, double heading, double angularVelocity, int minSpeed=JPR_MIN_PWM);
public:
    // omni configuration
    double degW[3];
    double wheelSpeeds[3];
    QVector2D vectWD[3];
    QVector2D vectWF[3];
    QVector2D calcCenterPoint;  //0,0 on keskel
    int wsi(int i){return (int)wheelSpeeds[i];}
    virtual void sendWheelSpeeds(){}
private:
    void omniVect(double velocity, double angularVelocity){omniVect(velocity,0,angularVelocity);}
    void omniVect(double vx, double vy, double w);
    double centerDistanceW(int i){return centerVector(i).length();}
    QVector2D centerVector(int i){return vectWD[i]-calcCenterPoint;}
    double angleBetween(QVector2D a, QVector2D b);
    void turnWheels();  // kui keskpunkt on nihutatud, siis on seda vaja
    void scaleWheelSpeeds();//{scaleWheelSpeeds(1);}    // väärtuseni üks
    void scaleWheelSpeeds(int n); // väärtuseni n
    void scaleWheelSpeedsOld(int n); // väärtuseni n, kui on üle selle, kasutusel vanas omnis
    void scaleToMinSpeed(double s);
    void scalePower();   // vana omnijuhtimise mingi skaleerimine, vist vajalik
public: void omniVana(float dirDeg, float velocityBody, int velocityMax, float velocityAngular);
private: void omniUuegaVana(float dirDeg, float velocityBody, int velocityMax, float velocityAngular);
};

#endif // JPROMNICONTROL_H
