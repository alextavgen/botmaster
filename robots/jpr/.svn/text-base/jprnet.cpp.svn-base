#include "jprnet.h"
#include <iostream>
#include "configfile.h"

using namespace std;

JprServer::JprServer(QObject *parent):QTcpServer(parent)
{
    connected = 0;
}

void JprServer::incomingConnection(int socketDescriptor)
{
        cout<<"incoming conection"<<endl;
        JprServerT *client = new JprServerT(socketDescriptor, this);
        connect(client, SIGNAL(finished()), client, SLOT(deleteLater()));
        //thread->start();

        QTcpSocket *tcpSocket = new QTcpSocket();
        if (!tcpSocket->setSocketDescriptor(socketDescriptor)) {
            emit error(tcpSocket->error());
            return;
        }
        connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
        QString str = QString("(%1) ")+tcpSocket->peerAddress().toString()+QString(":%2 ")+tcpSocket->peerName();
        str = str.arg(this->connected).arg(tcpSocket->peerPort());
        cout<<"Connected: "<<str.toStdString()<<endl;
        mClients[tcpSocket] = client;

}

void JprServer::readData()
{
    // retrieve our sender QTcpSocket
    QTcpSocket* tcpSocket = qobject_cast<QTcpSocket*>(sender());

    // get our Client
    JprServerT* c = mClients[tcpSocket];


    quint64 n=tcpSocket->bytesAvailable();
    char data[500];

    tcpSocket->read(data,n);

    cout<<"Read: "<<data;

    //parentx->SData.dataGetStrM(QString(data));
}


/////////////////////////


JprServerT::JprServerT(int socketDescriptor, QObject *parent) : QThread(parent), socketDescriptor(socketDescriptor), parentx((JprServer *)parent)
{
//        tcpSocket = new QTcpSocket();
//        connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
}

void JprServerT::run()
{

    tcpSocket = new QTcpSocket();
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));

    if (!tcpSocket->setSocketDescriptor(socketDescriptor)) {
        //emit error(tcpSocket->error());
        return;
    }
    QString str = QString("(%1) ")+tcpSocket->peerAddress().toString()+QString(":%2 ")+tcpSocket->peerName();
    str = str.arg(parentx->connected).arg(tcpSocket->peerPort());
    cout<<"Connected: "<<str.toStdString()<<endl;
    parentx->connected++;
    QByteArray block;
    int i=0;
    while(tcpSocket->state()==QAbstractSocket::ConnectedState){
        parentx->connected = true;
        QByteArray block;
        block.clear();
        //                parentx->SData.CmdID++;
        //                block.append(parentx->SData.dataSendStrM());
        //                block.append('\n');
        //                block.append((char)0);
        //                tcpSocket->write(block);
        //                tcpSocket->waitForBytesWritten();

        usleep(100*1000);
        //cout<<"Write: "<<block.data()<<endl;
        i++;
    }

    // klient võib ise ennast lahti ühendada, siis pole vaja seda serveril teha
    if(tcpSocket->state()==QAbstractSocket::ConnectedState){
        tcpSocket->disconnectFromHost();
        tcpSocket->waitForDisconnected();
    }
    cout<<"disConnected: "<<str.toStdString()<<endl;

    //parentx->connected = false;
    parentx->connected--;
}

void JprServerT::readData()
{
    quint64 n=tcpSocket->bytesAvailable();
    char data[500];

    tcpSocket->read(data,n);

    cout<<"Read: "<<data;

    //parentx->SData.dataGetStrM(QString(data));
}

//////////////////////////

JprNet::JprNet()
{
    server = new JprServer();
    ConfigFile cfg("./robots/jpr/cfg.txt");
    if (!server->listen(QHostAddress(cfg.HostIP),cfg.HostPort)) {
        cout<<"wat, ei läinud tööle\n";
    }else{
        cout<<"Server: "<<server->serverAddress().toString().toStdString()<<":"<<server->serverPort()<<endl;
    }
}
