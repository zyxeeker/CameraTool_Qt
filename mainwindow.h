#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAddressEntry>
#include "net/socket_test.h"
#include "param.h"
#include "logger/logger.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void RefreshDeviceList();

    void DeviceDetail();

    void GetLocalDetail();

    void SetLocal();

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    modu::socket_test m_socket;
    QMap<QString, Dev::Device> m_devices;

    Dev::Localhost m_localhost;
    Dev::Device m_cur_device;
};
#endif // MAINWINDOW_H
