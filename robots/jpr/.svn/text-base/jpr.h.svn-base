#ifndef JPR_H
#define JPR_H

#include "../../objecttype.h"
#include "../../comm.h"
#include "config.h"
#include "jprcontrol.h"
#include "jprmath.h"
#include "jprimage.h"

#include <iostream>
#include <fstream>
#include <QObject>
#include <QStringList>

#include "jprgyro.h"
enum ESearchStatus {ss_avoidWall,ss_avoidWallRight,ss_avoidWallLeft, ss_avoidGoal, ss_avoidRobot, ss_searchCircle, ss_quickTurn, ss_forward};
//enum EBoolState {bs_true = -2, bs_false = -1, bs_none = 0, bs_newFalse = 1, bs_newTrue = 2};

class Jpr: public JprImage {
    Q_OBJECT

public:
    Jpr();
    ~Jpr();

    virtual void go();

    QTime getGameTime(){return gameTime;}
    int getFrameCount(){return frameCount;}
    void finishDebug();
    void startDebug();
protected:
    Config  & conf;
    JprControl* jprControl;
    //JprMath* jprMath;
    JprStatClass* jprStatClass;
    //JprImage* jprImage;
    QTime time;
    QTime gameTime;
    QTime camReset;
    QTime ballTime;// aeg viimasest palli rulli alla saamisest
    int lastSeenBall;
    QTime pallEesTime;
    ESearchStatus searchBall;
    //Image::Object* pall;
    int frameCount;
    int lastFrameTime;
    int frameLength(){return gameTime.elapsed()-lastFrameTime;}
    char lastSeen;

    void soidaPallile();
    void soidaPallile2();
    void soidaPallile3();
    void penalt();

    void pallEes();
    bool lahedal;

    void doCircle(int,int);
    void avoidObstacles();
    float total;

    int aimStartTime; // väravale sihtimisel vaja robot peatada mingiks ajaks

    void printSensors();
    Image::Object* getGoal();
    void play();
    bool stop;

    void waitForStart();

    // logimine
    ofstream logFile;
    //QString versioon, veel ei tööta
    string logHeader();
    QStringList qstl;
    void logState();
    QString logSensors();
    QString logGoal();
    QString logBall();
    QString logPWM();

    //otse faili version
    void logState2();
    void logSensors2();
    void logGoal2();
    void logBall2();
    void logPWM2();

    int fps();
    int fpsTime;
    int fpsFrameCount;

    string curTime(){return QDateTime::currentDateTime().toString(QString("ddMMMyyyy_hhmmss")).toStdString();}
private:
    int sleepTime;
    QTime sleeper;
protected:
    string curCommandString;
public:
    void doNothing(int duration);
    void checkStateChanges();

    void getStateChange(bool cond,char &val);
    void getStateChange(bool cond,char &val, QTime &t);
    char pallEesChange;
    char varavChange;
    char pallChange;
    //char solBallApeared;

    bool waitForBallToLeave(int ms);

    JprGyro gyro;

    int kiirus_x;

    QTime soidaPallileAeg;

};

#endif
