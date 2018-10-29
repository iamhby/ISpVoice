#include "mainwindow.h"
#include <QApplication>
#include "voicethread.h"
#include "windows.h"
#include <sapi.h>
#include <QDebug>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();




    return a.exec();
}




