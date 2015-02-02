#ifndef JPRIMAGE_H
#define JPRIMAGE_H

#include "../../abstractrobot.h"
#include "jprcontrol.h"
#include "jprmath.h"
#include <QStack>
#include <iostream>

class JprObjekt{
public:
    JprObjekt(){centerX = 1; centerY = 1; width = 1;}
    JprObjekt(Image::Object *o){centerX = o->center.x; centerY = o->center.y; width = o->rect.width;}
    JprObjekt(const JprObjekt& x){centerX = x.centerX; centerY = x.centerY; width = x.width;}
    int centerX;
    int centerY;
    int width;
    void set(Image::Object *o){centerX = o->center.x; centerY = o->center.y; width = o->rect.width;}
};

class JprImage: public AbstractRobot, public QObject, public JprMath {
public:
    JprImage();
    ~JprImage();

    void processImage();

    CvVideoWriter* cvVW;

    Image::Object* getPall();
    void showGoal(int);
    void showImage();

    bool isValidKickPath();
    bool isValidKickPath2();
    int isValidKickPath3();
    Image::Object* getAnyGoal();
    bool isValidBall(Image::Object* obj, float k=0.1);

    virtual QTime getGameTime(){return QTime();}
    virtual int getFrameCount(){return 0;}
    virtual Image::Object* getGoal(){return NULL;}

    int imgFrameSkipCount;
    bool imgShowImages;

    char str[50];

public: 
    IplImage* img;
    Image::Object* pall;
    Image::Object* pallUnFiltered;  // mis objekti näeks ilma filtreerimatta
    Image::Object* goalRed;
    Image::Object* goalBlue;
    Image::Object* obj;


    Image::Object *prevPall;
    Image::Object *lockPall;
    int pallCount, prevPallCount;
    bool lockedPall;
    int i;

    int imgPallDir;
    int imgPallDirIgnore;
    int imgGoalRedDir;
    int imgGoalBlueDir;
    int goalDir;
    int imgPallDirTime;
    int imgGoalRedDirTime;
    int imgGoalBlueDirTime;
    int imgValidKickPath;
    //filtreerimine
    float imgFiltPall;  // kordaja, kui palju võib kaameras olev pall arvutuslikust erineda (0.1 = 10 protsenti)
    int imgMinGoalWidth;   // minimaalne värava laius pikslites
    int imgMinPallWidth;   // minimaalne palli laius pikslites, millest kindlasti allapoole ei saa minna

    CvFont font;
    static const double hScale=1.0;
    static const double vScale=1.0;
    static const int    lineWidth=1;

    int pallDif(Image::Object* p1, Image::Object* p2);
    int centerDif(Image::Object* p1, Image::Object* p2);
    int centerDif(JprObjekt p1, JprObjekt p2);
    void printBall(Image::Object *B);
    void printBallCalcs(Image::Object *B);

private:
    void colorGoal(Image::Object* g);
    JprObjekt prevRed;
    JprObjekt prevBlue;
    int difRed;
    int difBlue;
    Image::Object* iRedClose;
    Image::Object* iBlueClose;
};

#endif
