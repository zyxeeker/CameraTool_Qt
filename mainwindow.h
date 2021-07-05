#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAddressEntry>
#include "service/camera_detect.h"
#include "param.h"
#include "logger/logger.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void RefreshUVCDeviceList();

    void RefreshDeviceList();

    void DeviceDetail();

    void UVCDeviceDetail();

    void GetLocalDetail();

    void SetLocal();

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    CameraDetect m_socket;
    QMap<QString, Dev::NetDevice> m_netDevices;
    QMap<QString, Dev::UVCDevice> m_uvcDevices;

    Dev::Localhost m_localhost;
    Dev::NetDevice m_curNetDevice;
    Dev::UVCDevice m_curUVCDevice;


};
#endif // MAINWINDOW_H
