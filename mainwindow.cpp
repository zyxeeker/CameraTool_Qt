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
        ui->ip_input->setText(m_cur_device.ip);
        ui->gateway_input->setText(m_cur_device.gateway);
        ui->mask_input->setText(m_cur_device.mask);
    });
}

void MainWindow::DeviceDetail() {
    QListWidgetItem *item = ui->list->currentItem();

    LOG::logger(LOG::LogLevel::INFO, "Selected device Ipv4:" + item->text().toStdString());

    m_cur_device.ip = m_devices[item->text()].ip;
    m_cur_device.mask = m_devices[item->text()].mask;
    m_cur_device.gateway = m_devices[item->text()].gateway;
    m_cur_device.mac = m_devices[item->text()].mac;

    ui->device_ip->setText(m_cur_device.ip);
    ui->device_mac->setText(m_cur_device.mac);
    ui->device_gateway->setText(m_cur_device.gateway);
    ui->mask->setText(m_cur_device.mask);
}

void MainWindow::RefreshDeviceList() {
    LOG::logger(LOG::LogLevel::INFO, "Clean deivce`s list...");

    ui->list->clear();
    LOG::logger(LOG::LogLevel::INFO, "Start Refreshing...");

    m_socket.find_deivces();
    m_devices = m_socket.get_devices();
    if (m_devices.size() == 0)
        LOG::logger(LOG::LogLevel::WARN, "Not found any device.");

    for (auto iter : m_devices) {
        ui->list->addItem(iter.ip);
    }

    LOG::logger(LOG::LogLevel::INFO, "Refreshing list has finished!");
}

void MainWindow::GetLocalDetail() {
    QHostAddress ipv4_tmp;
    int32_t timer(0);
    QString gateway;
    QString local = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(local);
            foreach(QHostAddress address, info.addresses()) {
            //只取ipv4协议的地址
            if (address.protocol() == QAbstractSocket::IPv4Protocol)
                ipv4_tmp = address;
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
            // 遍历每一个IP地址
                    foreach(QNetworkAddressEntry entry, entryList) {
                    if (ipv4_tmp.toString() != entry.ip().toString()) continue;

                    m_localhost.name = netInterface.humanReadableName();
                    m_localhost.ip = entry.ip().toString();
                    m_localhost.mask = entry.netmask().toString();
                    m_localhost.gateway = gateway;
                    m_localhost.mac = netInterface.hardwareAddress();

                    ui->local_ip->setText(m_localhost.ip);
                    ui->local_mac->setText(m_localhost.mac);
                    ui->local_mask->setText(m_localhost.mask);
                    ui->local_gateway->setText(m_localhost.gateway);

                    LOG::logger(LOG::LogLevel::INFO, "Local Device Detail: IP:" + entry.ip().toString() +
                                                     " Netmask:" + entry.netmask().toString() +
                                                     " Broadcast:" + entry.broadcast().toString() +
                                                     " Gateway:" + gateway +
                                                     " Prefix Length:" + entry.prefixLength(), 0);
                }
        }
}

void MainWindow::SetLocal() {
    QRegExp rx("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    QString ip_input = ui->ip_input->text();
    QString gateway_input = ui->gateway_input->text();
    QString mask_input = ui->mask_input->text();

    if (ip_input.isEmpty() || gateway_input.isEmpty() || mask_input.isEmpty())
        ui->statusbar->showMessage("参数为空, 请键入参数！");
    else if (!rx.exactMatch(ip_input) || !rx.exactMatch(gateway_input) || !rx.exactMatch(mask_input))
        ui->statusbar->showMessage("非法参数!");
    QProcess cmd(this);
    QString command =
            "netsh interface ipv4 set address " + m_localhost.name + " static " + ip_input + " " + mask_input + " " +
            gateway_input;
    LOG::logger(LOG::LogLevel::INFO, command, 0);
    cmd.start(command);
    cmd.waitForFinished();
    ui->statusbar->showMessage("成功!");
}

MainWindow::~MainWindow() {
    delete ui;
}

