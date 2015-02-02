#include "sendthread.h"

SendThread::SendThread()
{
    server = new Server();
}

void SendThread::setImg( IplImage*  img){

  //server->frame=img;
    server->setFrame(img);
  //qDebug()<<img;
}
