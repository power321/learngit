#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>

class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    MyServer(QObject *parent = 0);
    ~MyServer();

private slots:
    void sendHeartBeat();
    void heartBeatTimeOut();
    void solveReadBuf();
    void solveError(QAbstractSocket::SocketError);

private:
    void incomingConnection(qintptr socketDescriptor) ;//Q_DECL_OVERRIDE;
    QTimer *myTimer, *timeOut;
    QTcpSocket *socket;
    quint32 nextBlockSize;
};

#endif // MYSERVER_H
