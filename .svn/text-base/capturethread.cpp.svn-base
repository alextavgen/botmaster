//#include "opencv/highgui.h"
#include "capturethread.h"
#include "QDebug"

CaptureThread::CaptureThread(){
    //returnFrame = cvCreateImage( cvSize(640, 480), IPL_DEPTH_8U, 3); //DEFAULT
}

IplImage * CaptureThread::getLatestFrame(){


    //returnFrame = cvCreateImage( cvGetSize(freshFrame), IPL_DEPTH_8U, 3);
    //returnFrame = cvCreateImage( cvSize(640, 480), IPL_DEPTH_8U, 3);
    //frameMutex.lock();
    //frameLock.lockForWrite();

    //RESETTING ROI of returnimage, to make it possible copy between picture from camera and img iplimage
    currentROI = cvGetImageROI(returnFrame);
    cvResetImageROI(returnFrame);

    if(lastWasBufferOne){
        //lastWasBufferOne = true;
        //qDebug() << cvGetSize(freshFrame_one).width << "x" << cvGetSize(freshFrame_one).height;
        //qDebug() << cvGetSize(returnFrame).width << "x" << cvGetSize(returnFrame).height;
        //qDebug() << "reading from one \n";

        cvCopy(freshFrame_one, returnFrame);
    }else{    
        //qDebug() << cvGetSize(freshFrame_two).width << "x" << cvGetSize(freshFrame_two).height;
        //qDebug() << cvGetSize(returnFrame).width << "x" << cvGetSize(returnFrame).height;
        //qDebug() << "reading from two \n";
        //lastWasBufferOne = false;
        cvCopy(freshFrame_two, returnFrame);
    }

    cvSetImageROI(returnFrame, currentROI);
    //frameLock.unlock();
    //frameMutex.unlock();

    //qDebug() << "Putting thread sleep for 10ms \n";
    //msleep(10);
    goSleep();
    //qDebug() << "Put thread sleep for 10ms \n";

    return returnFrame;
}

void CaptureThread::startAcquisition(CvCapture * setCap){
    cap = setCap;
    lastWasBufferOne = false;

    freshFrame_two = cvQueryFrame(cap); //DEFAULT VALUES
    freshFrame_one = cvQueryFrame(cap);

    returnFrame = cvCreateImage( cvGetSize(freshFrame_two), IPL_DEPTH_8U, 3);
    //returnFrame = cvCreateImage( cvSize(640, 480), IPL_DEPTH_8U, 3);

    //qDebug() << "FRESH1: " << cvGetSize(freshFrame_one).width << "x" << cvGetSize(freshFrame_one).height;
    //qDebug() << "FRESH2: " << cvGetSize(freshFrame_two).width << "x" << cvGetSize(freshFrame_two).height;
    //qDebug() << "RETURN: " << cvGetSize(returnFrame).width << "x" << cvGetSize(returnFrame).height;

    //qDebug() << "@init written to two \n";

    start(QThread::InheritPriority);
    //start(QThread::LowestPriority);
    //start(QThread::TimeCriticalPriority);
    qDebug() << "Started Capturing thread \n";
}

void CaptureThread::goSleep(){
    sleep = true;
}

void CaptureThread::run(){

    while(1){

        if(sleep = true){
            msleep(20);
            sleep = false;
        }
        //frameMutex.lock();
        //frameLock.lockForRead();

        if(!lastWasBufferOne){
            freshFrame_one = cvQueryFrame(cap);
            freshFrame_one = cvQueryFrame(cap);
            lastWasBufferOne = true;
            //qDebug() << "written to one \n";
        }else{
            freshFrame_two = cvQueryFrame(cap);
            freshFrame_two = cvQueryFrame(cap);
            lastWasBufferOne = false;
            //qDebug() << "written to two \n";
        }

        //frameMutex.unlock();
        //frameLock.unlock();
    }
}
