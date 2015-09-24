#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mythread.h"

#include <QDebug>
#include <QMessageBox>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stopButton->setEnabled(false);

    connect(ui->startButton, SIGNAL(clicked(bool)), this, SLOT(startPack()));
    connect(ui->stopButton, SIGNAL(clicked(bool)), this, SLOT(stopPack()));

}

int MainWindow::initSocket(int flag)
{
    /*
    if(flag == 0)
        return socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    return -1;
    */
}

void MainWindow::startPack()
{

    //const char *sIP = ui->addrSLEdit->text().toStdString().c_str();
    //const char *dIP = ui->addrDLEdit->text().toStdString().c_str();
    //qDebug()<<sIP<<dIP;
    myThread *mythread = new myThread(ui->addrSLEdit->text(), ui->addrDLEdit->text());
    connect(mythread, SIGNAL(updateSignal(QString, QString, QString, QString, QString, QString, QString, QString)), this, SLOT(updateSlot(QString, QString, QString, QString, QString, QString, QString, QString)));
    connect(this, SIGNAL(stopSignal()), mythread, SLOT(stopSlot()));
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    mythread->start();
}

void MainWindow::updateSlot(QString sIP, QString dIP, QString sMAC, QString dMAC, QString sPort, QString dPort, QString Protocol, QString Info)
{
    QTableWidgetItem *smacItem, *dmacItem, *sIpItem, *dIpItem, *sPortItem, *dPortItem, *protocolItem, *infoItem;
    int row_cnt = ui->outTable->rowCount();
    ui->outTable->insertRow(row_cnt);

    //IP Item init
    sIpItem = new QTableWidgetItem(sIP);
    ui->outTable->setItem(row_cnt, SRC_ADDR, sIpItem);
    dIpItem = new QTableWidgetItem(dIP);
    ui->outTable->setItem(row_cnt, DES_ADDR, dIpItem);

    //MAC Item init
    smacItem = new QTableWidgetItem(sMAC);
    ui->outTable->setItem(row_cnt, SRC_MAC, smacItem);
    dmacItem = new QTableWidgetItem(dMAC);
    ui->outTable->setItem(row_cnt, DES_MAC, dmacItem);

    //Port Item init
    sPortItem = new QTableWidgetItem(sPort);
    ui->outTable->setItem(row_cnt, SRC_PORT, sPortItem);
    dPortItem = new QTableWidgetItem(dPort);
    ui->outTable->setItem(row_cnt, DES_PORT, dPortItem);

    //Protocol Item init
    protocolItem = new QTableWidgetItem(Protocol);
    ui->outTable->setItem(row_cnt, PROTOCOL, protocolItem);

    //Info Item init
    infoItem = new QTableWidgetItem(Info);
    ui->outTable->setItem(row_cnt, INFO, infoItem);

    ui->outTable->resizeColumnsToContents();
    ui->outTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->outTable->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void MainWindow::stopPack()
{
    ui->stopButton->setEnabled(false);
    ui->startButton->setEnabled(true);
    emit stopSignal();
}

MainWindow::~MainWindow()
{
    delete ui;
}
