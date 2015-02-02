#include "server.h"
#include "protocolparser.h"
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <sstream>

using namespace std;

Server::Server()
{
    analogs = new int(8);
    digitals = new int(8);
    pildisuurus=0;
    pildisuurus2aste=0;

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    ipAddress = QHostAddress(QHostAddress("0.0.0.0")).toString();
    tcpServer = new QTcpServer(this);
    int port = 1234;
    if (!tcpServer->listen(QHostAddress(ipAddress), qint16(port))) {
        cout << "Kala! Serverit ei saa avada!" << endl;
    } else {

    }

    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address())
            ipAddress = ipAddressesList.at(i).toString();
    }

    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    cout << "Address: " << ipAddress.toStdString() << ":" <<
            tcpServer->serverPort() << endl;

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(connectionCreated()));

    //cam = cvCreateCameraCapture(-1);
    img = 0;
    frame = 0;

}

void Server::connectionCreated()
{

    clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));
    connect(clientConnection, SIGNAL(readyRead()),
            this, SLOT(processRead()));

    qDebug() << "Uus klient!";


}
void Server::processRead()
{

    QMutexLocker locker(&mutex);

    //cout << "processRead" << endl;
    QByteArray saadud = clientConnection->read(1024);

    unsigned char *data = (unsigned char*)saadud.data();
    //cout << "Parsimisele"<< data << endl;
    ProtocolParser pp;
    pp.parseData(data);

    if(pp.getCommand() == GET_ANALOGS) sendAnalogs();
    if(pp.getCommand() == GET_FRAME){

       // qDebug()<<frame;
        sendFrame(frame);
    }
    if(pp.getCommand() == GET_DIGITALS) sendDigitals();
}

void Server::sendFrame(IplImage* _frame)
{

    //qDebug() << "Hakkan frame saatma!";

    int height,width,step,channels;
    uchar *data;

    // frame = cvQueryFrame(cam);
    if (!_frame){
        qDebug()<<"Midagi pole veel saata";
        return;
    }

    img = cvCreateImage( cvGetSize(_frame), _frame->depth, _frame->nChannels );
    img->imageData = _frame->imageData;
    //cvResize(frame, img);

    if( !img ) return;
    // get the image data
    height    = img->height;
    width     = img->width;
    step      = img->widthStep;
    channels  = img->nChannels;
    data      = (uchar *)img->imageData;

    //pildisuurus=height*width*channels;
    //qDebug()<<width<<"x"<<height<<" "<<pildisuurus;
    //qDebug()<< " Tykk "<< j;

    /* pilditüki algusmarker */
    clientConnection->write("RF");

    clientConnection->write(QByteArray((char*)data,height*width*channels));
    //qDebug()<<"Olen siin!";
    clientConnection->flush();
    cvReleaseImage(&img);
    //qDebug()<<"Saadetud!";
}

void Server::sendAnalogs()
{
    QByteArray buf;
    char oo[255];

    clientConnection->write(QByteArray("RA"));
    for(int i = 0;i<8;i++){
    snprintf(oo,sizeof(oo),"%d,",analogs[i]);
    buf.append(oo);
    }

    clientConnection->write(buf,64);
    //qDebug() << "Sending analogs";

}

void Server::sendDigitals()
{

    QByteArray buf;

    //for(int i = 0;i<8;i++) digitals[i]=i%2;
    //cout << "Digital sent!" << endl;

    clientConnection->write(QByteArray("RD"));
    for(int i = 0; i < 8; i++){
        buf.append(digitals[i]+48);
    }
    qDebug()<<buf;
    clientConnection->write(buf);
}

void Server::setFrame(IplImage * img){

    frame=img;
   // qDebug()<<"Server frame=" << frame;
}

void Server::setAnalogs(int *_analogs)
{
    analogs = _analogs;
}

void Server::setDigitals(int *_digitals)
{
    digitals = _digitals;
}
Server::~Server()
{

    cvReleaseImage(&frame);
    cvReleaseCapture(&cam);
}
