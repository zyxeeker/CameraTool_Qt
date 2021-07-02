#include "mainwindow.h"
#include "socket_test.h"
#include <QApplication>

int main(int argc, char *argv[])
{
//    while(true){
//        modu::socket_test test1;
//        test1.find_deivces();
//        test1.get_devices();
//    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
