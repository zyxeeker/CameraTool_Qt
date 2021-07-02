#ifndef PARAM_H
#define PARAM_H

#define SX_CMD_SEARCH_ACK 0x2000

namespace Dev {
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

}

#endif // PARAM_H
