#include "mainwindow.h"

#include <QApplication>
#include "flowdatasource.h"
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
