#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QHostInfo>
#include <QNetworkAddressEntry>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    GetLocalDetail();
    RefreshDeviceList();

    connect(ui->do_refresh, &QPushButton::clicked, this, [=]() {
        RefreshDeviceList();
    });
    connect(ui->list, &QListWidget::itemClicked, this, [=]() {
        DeviceDetail();
    });
    connect(ui->local_refresh, &QPushButton::clicked, this, [=]() {
        GetLocalDetail();
    });

}

void MainWindow::DeviceDetail() {
    QListWidgetItem *item = ui->list->currentItem();
    qDebug() << "Selected device Ipv4:" << item->text();
    ui->device_ip->setText(m_devices[item->text()].ip);
    ui->device_mac->setText(m_devices[item->text()].mac);
    ui->device_gateway->setText(m_devices[item->text()].gateway);
    ui->mask->setText(m_devices[item->text()].mask);
}

void MainWindow::RefreshDeviceList() {
    qDebug() << "Clean deivce`s list...";
    ui->list->clear();
    qDebug() << "Start Refreshing...";
    m_socket.find_deivces();
    m_devices = m_socket.get_devices();
    if (m_devices.size() == 0)
        qDebug() << "Not found any device.";
    for (auto iter : m_devices) {
        ui->list->addItem(iter.ip);
    }
    qDebug() << "Refreshing list has finished!";
}

void MainWindow::GetLocalDetail() {
    QHostAddress ipv4_tmp;
    int32_t timer(0);
    QString gateway;
    QString local = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(local);

            foreach(QHostAddress address, info.addresses()) {
            //只取ipv4协议的地址
            if (address.protocol() == QAbstractSocket::IPv4Protocol) {
                ipv4_tmp = address;
                qDebug() << address.toString();
            }
        }
    gateway = ipv4_tmp.toString();
    for (int i = 0; i < gateway.size(); ++i) {
        if (timer == 3)
            gateway.replace(i, gateway.size() - i, QString("1"));
        if (timer != 3 && gateway[i].isPunct())
            ++timer;
    }

    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
            foreach (QNetworkInterface netInterface, list) {
            QList<QNetworkAddressEntry> entryList = netInterface.addressEntries();
            qDebug() << netInterface.hardwareAddress();
            // 遍历每一个IP地址
                    foreach(QNetworkAddressEntry entry, entryList) {
                    if (ipv4_tmp.toString() != entry.ip().toString()) continue;
                    ui->local_ip->setText(entry.ip().toString());
                    ui->local_mac->setText(netInterface.hardwareAddress());
                    ui->local_mask->setText(entry.netmask().toString());
                    ui->local_gateway->setText(gateway);
                    qDebug() << "******Local Device Detail******";
                    // IP地址
                    qDebug() << "IP Address:" << entry.ip().toString();
                    // 子网掩码
                    qDebug() << "Netmask:" << entry.netmask().toString();
                    // 广播地址
                    qDebug() << "Broadcast:" << entry.broadcast().toString();
                    // 网关
                    qDebug() << "Gateway:" << gateway;
                    // 前缀长度
                    qDebug() << "Prefix Length:" << entry.prefixLength();
                    qDebug() << "*******************************";
                }
        }
}


MainWindow::~MainWindow() {
    delete ui;
}

