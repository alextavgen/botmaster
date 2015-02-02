#ifndef MAIUDEBUG_H
#define MAIUDEBUG_H

#include <QString>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <cv.h>
#include <highgui.h>
#include "image.h"
#include <QTime>

//TODO Mis asi see on
#define STOP 0
//TODO Mis asi see on
#define OTSI_PALLI 1
//TODO Mis asi see on
#define POORA_PALLILE 2
//TODO Mis asi see on
#define SOIDA_PALLINI 3
//TODO Mis asi see on
#define OTSI_VARAVAT 4
//TODO Mis asi see on
#define POORA_VARAVALE 5

class MaiuDebug
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

    MaiuDebug();
    void alustaLogimist( int debugFile, int degbugVideo);
    void logi( int debugFile, int debugVideo, IplImage* img, Image::Object* pall, int analog[], int digital[], int pwms[], int state, int kaader, int ms );
    void lopetaLogimine( int debugFile, int degbugVideo );
    QString getCurrentTime(){
        return QDateTime::currentDateTime().toString( QString("dd:MMM:yyyy_hh:mm:ss") );
    }
};

#endif // MAIUDEBUG_H
