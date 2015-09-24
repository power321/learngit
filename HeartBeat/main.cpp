#include "mainwindow.h"
#include "myserver.h"
#include <QApplication>

#define PORT 5555

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyServer server;
    if (!server.listen(QHostAddress::Any, PORT)){
        qDebug("listen error!");
        return 1;
    } else{
        qDebug("Listenning ... ");
    }
    MainWindow w;
    w.show();

    return a.exec();
}
