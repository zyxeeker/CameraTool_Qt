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

    void find_uvc_devices();

    QMap<QString, Dev::UVCDevice> getUVCDevices() const { return m_uvcDeviceList; }
private:
    QMap<QString, Dev::UVCDevice> m_uvcDeviceList;
    Dev::UVCDevice m_uvcDevice;
};


#endif // SOCKET_TEST_H
