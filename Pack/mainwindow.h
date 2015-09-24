#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void stopSignal();

private slots:
    void startPack();
    void stopPack();
    void updateSlot(QString sIP, QString dIP, QString sMAC, QString dMAC, QString sPort, QString dPort, QString Protocol, QString Info);

private:
    Ui::MainWindow *ui;

    enum{
        SRC_ADDR,
        DES_ADDR,
        SRC_MAC,
        DES_MAC,
        SRC_PORT,
        DES_PORT,
        PROTOCOL,
        INFO
    };
    int socketID;

    int initSocket(int);
};

#endif // MAINWINDOW_H
