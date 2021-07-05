#ifndef SOCKET_TEST_H
#define SOCKET_TEST_H

#include <QTcpSocket>
#include <QUdpSocket>
#include <QDebug>
#include "param.h"

class CameraDetect {
public:
    CameraDetect();

    ~CameraDetect();

    void find_deivces();

    QMap<QString, Dev::Device> get_devices() const { return m_device_list; }

private:
    QUdpSocket *m_udp_socket = nullptr;
    QMap<QString, Dev::Device> m_device_list;
    Dev::Device m_device;
};


#endif // SOCKET_TEST_H
