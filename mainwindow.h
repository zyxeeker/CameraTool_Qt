#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAddressEntry>
#include "socket_test.h"
#include "param.h"

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

    QString m_ip_tmp;
    QString m_gateway_tmp;
    QString m_mask_tmp;
    QString m_net_name;
};
#endif // MAINWINDOW_H
