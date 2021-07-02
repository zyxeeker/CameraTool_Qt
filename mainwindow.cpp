#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->do_refresh,&QPushButton::clicked,this,[=](){
        refresh_list();
    });
    connect(ui->list,&QListWidget::itemClicked,this,[=](){
        device_detail();
    });
}

void MainWindow::device_detail(){
    QListWidgetItem *item = ui->list->currentItem();
    qDebug()<<"Selected device Ipv4:"<<item->text();
    ui->device_ip->setText(m_devices[item->text()].ip);
    ui->device_mac->setText(m_devices[item->text()].mac);
    ui->device_gateway->setText(m_devices[item->text()].gateway);
    ui->mask->setText(m_devices[item->text()].mask);
}

void MainWindow::refresh_list(){
    qDebug()<<"Clean deivce`s list...";
    ui->list->clear();
    qDebug()<<"Start Refreshing...";
//    m_socket = new modu::socket_test;
    m_socket.find_deivces();
    m_devices = m_socket.get_devices();
    if(m_devices.size()==0)
        qDebug()<<"Not found any device.";
    for(auto iter : m_devices){
//        qDebug()<<iter;
        ui->list->addItem(iter.ip);
    }
    qDebug()<<"Refreshing list has finished!";
}


MainWindow::~MainWindow()
{
    delete ui;
}

