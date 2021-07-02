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
        device_detail(0);
    });
}

void MainWindow::device_detail(int index){
    int currenRow = ui->list->currentRow();
    qDebug()<<currenRow;
    QListWidgetItem *item = ui->list->;
    qDebug()<<item->data(currenRow);
}

void MainWindow::refresh_list(){
    qDebug()<<"Clean deivce`s list...";
    ui->list->clear();
    qDebug()<<"Start Refreshing...";
    m_socket = new modu::socket_test;
    m_socket->find_deivces();
    m_devices = m_socket->get_devices();
    if(m_devices.size()==0)
        qDebug()<<"Not found any device.";
    for(auto iter : m_devices){
        ui->list->addItem(QString("sasa"));
    }
    qDebug()<<"Refreshing list has finished!";
}


MainWindow::~MainWindow()
{
    delete ui;
}

