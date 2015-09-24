#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void acceptConnect();
    void readMessage();
    void sendMessage(const char *);

private:
    Ui::MainWindow *ui;
    QTcpSocket *clientSocket;
    quint32 nextBlockSize;
};

#endif // MAINWINDOW_H
