#ifndef JPRNET_H
#define JPRNET_H

#include <QtNetwork>
#include <QTcpServer>
#include <QObject>

// siia tuleb kood üle tcp debugimiseks

class JprServerT;

class JprServer: public QTcpServer
{
    Q_OBJECT
public:
    JprServer(QObject *parent = 0);
    int connected;
    QMap<QTcpSocket*, JprServerT*> mClients;
protected:
    void incomingConnection(int socketDescriptor);
private:

signals:
    void error(QTcpSocket::SocketError socketError);
private slots:
    void readData();
};

//////////////////////////////////////
// kliendi thread
class JprServerT : public QThread
{
    Q_OBJECT

public:
    JprServerT(int socketDescriptor, QObject *parent);
    void run();
signals:

private slots:
    void readData();
protected:
    void incomingConnection(int socketDescriptor);
private:
    int socketDescriptor;
    JprServer *parentx;
    QTcpSocket *tcpSocket;
};

///////////////////////////////////////
class JprNet
{
public:
    JprNet();
    JprServer *server;
};


#endif // JPRNET_H
