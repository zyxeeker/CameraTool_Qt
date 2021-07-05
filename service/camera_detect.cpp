#include "camera_detect.h"
#include "logger/logger.h"
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

#define SX_CMD_SEARCH_REQ 0x1000

CameraDetect::CameraDetect() {
}

CameraDetect::~CameraDetect() {
    m_udp_socket->close();
}

void CameraDetect::find_net_devices() {
    if (m_udp_socket) m_udp_socket->close();
    m_udp_socket = new QUdpSocket();

    Dev::BroadcastReq req;
    QByteArray datagram;

    req.cmd = htonl(SX_CMD_SEARCH_REQ);
    req.length = htonl(sizeof(Dev::BroadcastReq));
    req.id = qrand();
    memset(req.rev, 0, 32);

    m_udp_socket->bind(15120, QUdpSocket::ShareAddress);

    m_udp_socket->writeDatagram(reinterpret_cast<char*>(&req), sizeof(Dev::BroadcastReq), QHostAddress::Broadcast,15120);

    while(m_udp_socket->hasPendingDatagrams()) {
        datagram.resize(m_udp_socket->pendingDatagramSize());
        m_udp_socket->readDatagram(datagram.data(), datagram.size());
        Dev::BroadcastResp *resp = reinterpret_cast<Dev::BroadcastResp *>(datagram.data());
        uint8_t *ptr_d = reinterpret_cast<uint8_t *>(resp);
        LOG::logger(LOG::LogLevel::INFO, "CameraDetectService: received broadcast,"
                                         " flag:" + QString(resp->flag) +
                                         " cmd:" + ntohl(resp->cmd) +
                                         " length:" + ntohl(resp->length) +
                                         " ipv4:" + resp->ipv4, 0);

        QString ipv4 = resp->ipv4;
        QStringList ethes = ipv4.split(";");

        if (ethes.size() != 0) {
            for (QString item : ethes) {
                if (item.size() > 0) {
                    QStringList info = item.split("#");
                    QString ip = info[2];
                    m_netDevice.mac = info[1];
                    m_netDevice.ip = info[2];
                    m_netDevice.gateway = info[3];
                    m_netDevice.mask = info[4];
                    m_netDeviceList[ip] = m_netDevice;

                }
            }
        }

    }
}
