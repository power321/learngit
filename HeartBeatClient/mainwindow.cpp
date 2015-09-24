#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHostAddress>
#define PORT 5555
#define BUFSIZE 1024 * 8

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    clientSocket = new QTcpSocket(this);
    nextBlockSize = 0;
    connect(clientSocket, SIGNAL(connected()), this, SLOT(acceptConnect()));
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    connect(clientSocket, SIGNAL(disconnected()), clientSocket, SLOT(deleteLater()));
    clientSocket->connectToHost(QHostAddress::LocalHost, PORT);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::acceptConnect()
{
    qDebug("connect ok!");
}

void MainWindow::readMessage()
{
    QDataStream in(clientSocket);
    in.setVersion(QDataStream::Qt_5_4);

    if (nextBlockSize == 0) {
       if (clientSocket->bytesAvailable() < sizeof(quint32))
           return ;
       in >> nextBlockSize;
    }
    if (nextBlockSize > clientSocket->bytesAvailable())
        return ;
    char readBuf[BUFSIZE];
    int readLen = in.readRawData(readBuf, nextBlockSize);
    if (readLen <= 0 ){
        qDebug("read error!");
        return ;
    } else{
        qDebug("read %s", readBuf);
    }
    if (strcmp(readBuf, "chenxian") == 0){
        this->sendMessage("chenxian");
    } else{
        this->sendMessage("Unknown");
    }
    nextBlockSize = 0;
}

void MainWindow::sendMessage(const char *message)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_4);

    const char *tmpBuf = message;
    out << tmpBuf;
    clientSocket->write(block);
}
