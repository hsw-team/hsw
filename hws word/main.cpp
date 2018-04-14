#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
//    MyThread thr;

    w.show();
//    thr.start();
    return a.exec();
}
