#ifndef SOCKET_TEST_H
#define SOCKET_TEST_H

#include <QTcpSocket>
#include <QUdpSocket>
#include <QDebug>
#include <QCameraInfo>
#include "param.h"

class CameraDetect {
public:
    CameraDetect();

    ~CameraDetect();

    void find_net_devices();

    void find_uvc_devices();

    QMap<QString, Dev::NetDevice> getNetDevices() const { return m_netDeviceList; }

    QMap<QString, Dev::UVCDevice> getUVCDevices() const { return m_uvcDeviceList; }

private:
    QUdpSocket *m_udpSocket = nullptr;
    QMap<QString, Dev::NetDevice> m_netDeviceList;
    Dev::NetDevice m_netDevice;

    QMap<QString, Dev::UVCDevice> m_uvcDeviceList;
    Dev::UVCDevice m_uvcDevice;
};


#endif // SOCKET_TEST_H
