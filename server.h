#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>
#include <QMutex>
#include <cv.h>
#include <highgui.h>

class Server : public QTcpServer
{
   Q_OBJECT
public:
   Server();
   ~Server();
   IplImage* frame;
   void setFrame(IplImage*);
   void setAnalogs(int *);
   void setDigitals(int *);

public slots:
   void connectionCreated();
   void processRead();
   void sendFrame(IplImage*);
   void sendAnalogs();
   void sendDigitals();

private:
   int *analogs;
   int *digitals;
   int pildisuurus; //pildi suurus
   int pildisuurus2aste; //pildi suurus kahe astmena
   QTcpServer *tcpServer;
   QTcpSocket *clientConnection;
   QMutex mutex;
   void sendData();
   CvCapture *cam;
   IplImage* img;



};

#endif // SERVER_H
