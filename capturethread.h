#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H


// Qt header files
#include <QThread>
#include <QMutex>
#include <QReadWriteLock>
// #include <QtGui> // ??

// OpenCV header files
#include "opencv/highgui.h"
#include <cv.h>

class CaptureThread : public QThread
{

    public:
    CaptureThread();
   // ~CaptureThread();

    IplImage * getLatestFrame();

    void startAcquisition(CvCapture *);


    private:
    QMutex frameMutex;
    QReadWriteLock frameLock;
    CvRect currentROI;

    bool lastWasBufferOne;
    bool sleep;
    void goSleep();

    IplImage * freshFrame_one;
    IplImage * freshFrame_two;

    IplImage * returnFrame;
    CvCapture * cap;


    protected:
    void run();
};

#endif // CAPTURETHREAD_H
