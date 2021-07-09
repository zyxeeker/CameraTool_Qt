#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QHostInfo>
#include <QMessageBox>
#include <QProcess>

void MainWindow::Display(cv::Mat frame) {
//    if (frame.empty())
//        ui->close_btn->setEnabled(false);
//    else
//        ui->close_btn->setEnabled(true);
    QImage Img;
    cv::cvtColor(frame, frame, CV_RGB2BGR);
    Img = QImage((const uchar*)(frame.data), frame.cols, frame.rows, frame.cols*frame.channels(), QImage::Format_RGB888);
    QPixmap tmp = QPixmap::fromImage(Img);
    ui->frame->setPixmap(tmp);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    m_cameraRecord = new CameraRecord(480, 640, 30.0);

    qRegisterMetaType<cv::Mat>("cv::Mat");

    GetLocalDetail();

    RefreshDeviceList();
    RefreshUVCDeviceList();

    connect(&m_cameraCore, SIGNAL(SendFrame(cv::Mat)),this, SLOT(Display(cv::Mat)));
    connect(this, SIGNAL(SendStatue(bool)), &m_cameraCore, SLOT(SetPreviewStatue(bool)));
    connect(this, SIGNAL(SetRecordStatue(bool)), &m_cameraCore, SLOT(_SetRecordStatue(bool)));
    connect(&m_cameraCore, SIGNAL(SendFrame2Record(cv::Mat)),m_cameraRecord, SLOT(GetFrame(cv::Mat)));

    connect(ui->do_refresh, &QPushButton::clicked, this, [=]() {
        RefreshDeviceList();
    });
    connect(ui->do_uvc_refresh, &QPushButton::clicked, this, [=]() {
        RefreshUVCDeviceList();
    });
    connect(ui->uvc_list, &QListWidget::itemClicked, this, [=]() {
        UVCDeviceDetail();
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
        ui->ip_input->setText(m_curNetDevice.ip);
        ui->gateway_input->setText(m_curNetDevice.gateway);
        ui->mask_input->setText(m_curNetDevice.mask);
    });
    connect(ui->open_btn, &QPushButton::clicked, this, [=](){
        if (m_cameraCore.isRunning())
            emit SendStatue(true);
        else
            m_cameraCore.start();
        ui->open_btn->setEnabled(false);
        ui->close_btn->setEnabled(true);
        ui->start_btn->setEnabled(true);
        ui->rotate_btn->setEnabled(true);
    });
    connect(ui->close_btn, &QPushButton::clicked, this, [=](){
        emit SendStatue(false);
        ui->close_btn->setEnabled(false);
        ui->open_btn->setEnabled(true);
        ui->start_btn->setEnabled(false);
        ui->pause_btn->setEnabled(false);
        ui->stop_btn->setEnabled(false);
        ui->rotate_btn->setEnabled(false);
    });
    connect(ui->start_btn, &QPushButton::clicked, this, [=](){
        m_cameraRecord->start();
        emit SetRecordStatue(true);
        ui->start_btn->setEnabled(false);
        ui->pause_btn->setEnabled(true);
        ui->stop_btn->setEnabled(true);
        ui->rotate_btn->setEnabled(true);
    });
    connect(ui->pause_btn, &QPushButton::clicked, this, [=](){
        ui->pause_btn->setText("继续");

    });
    connect(ui->stop_btn, &QPushButton::clicked, this, [=](){
        emit SetRecordStatue(false);
        m_cameraRecord->t1 = false;
        ui->start_btn->setEnabled(true);
        ui->stop_btn->setEnabled(false);
        ui->pause_btn->setEnabled(false);
    });
    connect(ui->rotate_btn, &QPushButton::clicked, this, [=](){

    });

}

void MainWindow::DeviceDetail() {
    QListWidgetItem *item = ui->list->currentItem();

    LOG::logger(LOG::LogLevel::INFO, "Selected device Ipv4:" + item->text().toStdString());

    m_curNetDevice = m_netDevices[item->text()];

    ui->device_ip->setText(m_curNetDevice.ip);
    ui->device_mac->setText(m_curNetDevice.mac);
    ui->device_gateway->setText(m_curNetDevice.gateway);
    ui->mask->setText(m_curNetDevice.mask);
}

void MainWindow::UVCDeviceDetail() {
    QListWidgetItem *item = ui->uvc_list->currentItem();
    m_curUVCDevice = m_uvcDevices[item->text()];

    ui->uvc_des->setText(m_curUVCDevice.des);
    ui->p_1->setText(m_curUVCDevice.p1);
    ui->p_2->setText(m_curUVCDevice.p2);
    ui->p_3->setText(m_curUVCDevice.p3);
    ui->p_4->setText(m_curUVCDevice.p4);
}

void MainWindow::RefreshDeviceList() {
    LOG::logger(LOG::LogLevel::INFO, "Clean deivce`s list...");

    ui->list->clear();
    LOG::logger(LOG::LogLevel::INFO, "Start Refreshing...");

    m_detectService.find_net_devices();
    m_netDevices = m_detectService.getNetDevices();
    if (m_netDevices.size() == 0)
        LOG::logger(LOG::LogLevel::WARN, "Not found any device.");

    for (auto iter : m_netDevices)
        ui->list->addItem(iter.ip);

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
    else {
        QProcess cmd(this);
        QString command =
                "netsh interface ipv4 set address " + m_localhost.name + " static " + ip_input + " " + mask_input +
                " " +
                gateway_input;
        LOG::logger(LOG::LogLevel::INFO, command, 0);
        cmd.start(command);
        cmd.waitForFinished();
        ui->statusbar->showMessage("成功!");
    }
}

void MainWindow::RefreshUVCDeviceList() {
    ui->uvc_list->clear();
    m_detectService.find_uvc_devices();
    m_uvcDevices = m_detectService.getUVCDevices();

    for (auto item : m_uvcDevices)
        ui->uvc_list->addItem(item.des);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (m_cameraCore.isRunning()) {
        if (m_cameraCore.GetCurMark()) {
            if (QMessageBox::warning(NULL, "注意", "请停止录制后再退出！", QMessageBox::Close) == QMessageBox::Close)
                event->ignore();
        }
        else
            m_cameraCore.terminate();
    }
}