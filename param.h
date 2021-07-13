#ifndef PARAM_H
#define PARAM_H

#include <cstdint>
#include <QString>

#define SX_CMD_SEARCH_ACK 0x2000

namespace Dev {
    typedef struct UVCDevice {
        QString p1;
        QString p2;
        QString p3;
        QString p4;
        QString des;
    } UVCDevice;
}

#endif // PARAM_H
