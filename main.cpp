#include "mainwindow.h"
//#include "types.h"
//#include "SLABCP2112.h"
//#include "smbus.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
