#include "mainwindow.h"
#include "service/camera_detect.h"
#include "service/camera_core.h"
#include <QApplication>

int main(int argc, char *argv[])
{
//    CameraCore tmp;
//    tmp.OpenCamera_test();
//    while(true){
//        modu::camera_detect test1;
//        test1.find_deivces();
//        test1.get_devices();
//    }
//
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
