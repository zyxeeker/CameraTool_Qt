#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QHostInfo>
#include <QMessageBox>
#include <QProcess>

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
    connect(ui->apply, &QPushButton::clicked, this, [=]() {
        SetLocal();
    });
    connect(ui->direct, &QRadioButton::clicked, this, [=]() {
        QMessageBox::information(NULL, "注意", "请直连设备！", QMessageBox::Ok, QMessageBox::Ok);
        ui->ip_input->setText(m_ip_tmp);
        ui->gateway_input->setText(m_gateway_tmp);
    });
}

void MainWindow::DeviceDetail() {
    QListWidgetItem *item = ui->list->currentItem();
    qDebug() << "Selected device Ipv4:" << item->text();
    m_ip_tmp = m_devices[item->text()].ip;
    m_gateway_tmp = m_devices[item->text()].gateway;

    ui->device_ip->setText(m_ip_tmp);
    ui->device_mac->setText(m_devices[item->text()].mac);
    ui->device_gateway->setText(m_gateway_tmp);
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

            qDebug() << m_net_name;
            // 遍历每一个IP地址
                    foreach(QNetworkAddressEntry entry, entryList) {
                    if (ipv4_tmp.toString() != entry.ip().toString()) continue;

                    m_net_name = netInterface.humanReadableName();

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

void MainWindow::SetLocal() {
    QRegExp rx("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    QString ip_tmp = ui->ip_input->text();
    QString gateway_tmp = ui->gateway_input->text();
    QString mask_tmp = ui->mask_input->text();

    if (ip_tmp.isEmpty() || mask_tmp.isEmpty())
        ui->statusbar->showMessage("参数为空, 请键入参数！");
    else if (!rx.exactMatch(ip_tmp))
        ui->statusbar->showMessage("非法参数!");
    qDebug() << m_net_name;
    QProcess cmd(this);
    QString command =
            "netsh interface ipv4 set address " + m_net_name + " static " + ip_tmp + " " + mask_tmp + " " + gateway_tmp;
    qDebug() << command;
    cmd.start(command);
    ui->statusbar->showMessage("waiting...");
    cmd.waitForFinished();
}

MainWindow::~MainWindow() {
    delete ui;
}

