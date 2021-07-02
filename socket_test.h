#ifndef SOCKET_TEST_H
#define SOCKET_TEST_H

#include<QTcpSocket>
#include<QUdpSocket>
#include<QDebug>

#define SX_CMD_SEARCH_ACK 0x2000

namespace modu {
typedef struct BroadcastReq
{
    char flag[4] = {'s','e','e','v'};
    uint32_t length = 48;
    uint32_t cmd = 0x1000;
    uint32_t id = 0;
    char rev[32];
}BroadcastReq;

typedef struct BroadcastResp
{
    char flag[4] = {'s','e','e','v'};
    uint32_t length = 288;
    uint32_t cmd = SX_CMD_SEARCH_ACK;
    uint32_t id = 0;
    char sn[16];
    char ipv4[256];
}BroadcastResp;

typedef struct Device{
    QString ip;
    QString mac;
    QString gateway;
    QString mask;
} Device;

class socket_test{
public:
    socket_test();
    ~socket_test();
    void find_deivces();
    QMap<QString, modu::Device> get_devices() const {return m_device_list;}
private:
    QUdpSocket *m_udp_socket = nullptr;
    QMap<QString, modu::Device> m_device_list;
    modu::Device m_device;

};
}


#endif // SOCKET_TEST_H
