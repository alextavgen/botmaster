#ifndef HALDEBUG_H
#define HALDEBUG_H

#include <QString>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <cv.h>
#include <highgui.h>
#include "image.h"
#include <QTime>

#define STOP 0
#define OTSI_PALLI 1
#define POORA_PALLILE 2
#define SOIDA_PALLINI 3
#define OTSI_VARAVAT 4
#define POORA_VARAVALE 5

class HalDebug
{
public:

    // logi buffer
    char str[50];
    // faili logimine
    QFile* data;
    QTextStream* out;
    // video kirjutaja
    CvVideoWriter* cvVW;
    // log font
    CvFont font;

    HalDebug();
    void alustaLogimist( int debugFile, int degbugVideo);
    void logi( int debugFile, int debugVideo, IplImage* img, Image::Object* pall, int analog[], int digital[], int pwms[], int state, int kaader, int ms );
    void lopetaLogimine( int debugFile, int degbugVideo );
    QString getCurrentTime(){
        return QDateTime::currentDateTime().toString( QString("dd:MMM:yyyy_hh:mm:ss") );
    }
};

#endif // HALDEBUG_H
