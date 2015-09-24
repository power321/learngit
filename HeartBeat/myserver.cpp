#include "myserver.h"
#include <cstring>
#include <QTime>
#include <QDebug>

#define BUFSIZE 1024 * 8

MyServer::MyServer(QObject *parent)
    : QTcpServer(parent), nextBlockSize(0)
{
    myTimer = new QTimer(this);
    timeOut = new QTimer(this);
}

MyServer::~MyServer()
{
    delete myTimer;
    myTimer = NULL;
    delete timeOut;
    timeOut = NULL;
}

void MyServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug("incoming ... ");
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, SIGNAL(readyRead()), this, SLOT(solveReadBuf()));
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));

    connect(this, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(solveError(QAbstractSocket::SocketError)));

    connect(myTimer, SIGNAL(timeout()), this, SLOT(sendHeartBeat()));
    connect(timeOut, SIGNAL(timeout()), this, SLOT(heartBeatTimeOut()));
    myTimer->start(1000 * 15);
    qDebug("start Timer ... ");
}

void MyServer::solveReadBuf()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_4);

    if (nextBlockSize == 0) {
       if (socket->bytesAvailable() < sizeof(quint32))
           return ;
       in >> nextBlockSize;
    }
    if (nextBlockSize > socket->bytesAvailable())
        return ;
    char readBuf[BUFSIZE];
    int readLen = in.readRawData(readBuf, nextBlockSize);
    if (readLen <= 0 ){
        qDebug("read error!");
        return ;
    } else{
        qDebug("read %s",readBuf);
    }
    if (strcmp(readBuf, "chenxian") == 0){
        qDebug("stop timeout!");
        timeOut->stop();
    }
    nextBlockSize = 0;
}

void MyServer::sendHeartBeat()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_4);

    const char *tmpBuf = "chenxian";
    out << tmpBuf;
    socket->write(block);
    timeOut->start(1000 * 5);
}

void MyServer::heartBeatTimeOut()
{
    QTime now = QTime::currentTime();
    int minu = now.minute();
    int sec = now.second();
    qDebug("HeartBeatTimeOut!");
    qDebug()<<"minute:"<<minu<<"sec:"<<sec;
}

void MyServer::solveError(QAbstractSocket::SocketError error)
{
    //qDebug()<<error;
    switch (error) {
    case 0:
        qDebug("connection refuse or timeout!");
        break;
    default:
        qDebug("other error!");
        break;
    }
}
