#include "socket_test.h"
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

#define SX_CMD_SEARCH_REQ 0x1000

modu::socket_test::socket_test(){

}

void modu::socket_test::find_deivces(){
    modu::BroadcastReq req;
    QByteArray datagram;

    req.cmd = htonl(SX_CMD_SEARCH_REQ);
    req.length = htonl(sizeof (modu::BroadcastReq));
    req.id = qrand();
    memset(req.rev,0,32);
    m_udp_socket = new QUdpSocket();
    m_udp_socket->bind(15120, QUdpSocket::ShareAddress);
    m_udp_socket->writeDatagram(reinterpret_cast<char*>(&req), sizeof(modu::BroadcastReq), QHostAddress::Broadcast,15120);
    while(m_udp_socket->hasPendingDatagrams()){
        datagram.resize(m_udp_socket->pendingDatagramSize());
        m_udp_socket->readDatagram(datagram.data(),datagram.size());
        modu::BroadcastResp* resp = reinterpret_cast<modu::BroadcastResp*>(datagram.data());
        uint8_t* ptr_d = reinterpret_cast<uint8_t*>(resp);
        qDebug()<<"CameraDetectService: received broadcast,"<<
                  "flag:"<<resp->flag<<
                  "cmd:"<<ntohl(resp->cmd)<<
                  "length:"<<ntohl(resp->length)<<
                  "ipv4:"<<resp->ipv4;

        QString ipv4 = resp->ipv4;
        QStringList ethes = ipv4.split(";");

        if(ethes.size()!=0){
            qDebug()<<ethes;
            for(QString item : ethes){
                if(item.size()>0){
                    QStringList info =  item.split("#");
                    qDebug()<<info;
                    QString mac = info[1];
                    QString ip = info[2];
                    m_devices[ip] = mac;
                }
            }
        }

    }
    m_udp_socket->close();
}
