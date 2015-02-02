#ifndef SENDTHREAD_H
#define SENDTHREAD_H

#include <QThread>
#include <QtNetwork>
#include "server.h"
#include <cv.h>

class SendThread : public QThread
{
public:
    SendThread();
    void setImg(IplImage*);
    Server *server;

private:


};

#endif // SENDTHREAD_H
