#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QProcess>
#include <QPainter>

void MainWindow::Display(cv::Mat frame) {
//    if (!frame.empty())
//        ui->close_btn->setEnabled(true);
    cv::Mat tmp, tmp_copy;
    QImage Img, Img_copy;
    double x;
    double y;

    cv::cvtColor(frame, tmp, CV_RGB2BGR);
    Img = QImage((const uchar *) (tmp.data), tmp.cols, tmp.rows, tmp.cols * frame.channels(), QImage::Format_RGB888);

    if (m_X != -1 || m_Y != -1) {
        tmp_copy = tmp.clone();
#if FRAME_TEST
        if (m_XTmp != m_X || m_YTmp != m_Y){
            m_XTmp = m_X;
            m_YTmp = m_Y;
#endif
        double w = double(tmp.cols) / double(ui->frame->width());
        double h = double(tmp.rows) / double(ui->frame->height());

        x = w * m_X;
        y = h * m_Y;
#if FRAME_TEST
        }
#endif
        if (x >= tmp_copy.cols || y >= tmp_copy.rows) {
            x = m_LastX;
            y = m_LastY;
        } else {
            m_LastX = x;
            m_LastY = y;
        }

        cv::line(tmp_copy, cv::Point(x - 10 / 2, y), cvPoint(x + 10 / 2, y), CV_RGB(0, 255, 255), 2, 8, 0);
        //绘制竖线
        cv::line(tmp_copy, cvPoint(x, y - 10 / 2), cvPoint(x, y + 10 / 2), CV_RGB(0, 255, 255), 2, 8, 0);

        int R = tmp.at<cv::Vec3b>(y, x)[0];
        int G = tmp.at<cv::Vec3b>(y, x)[1];
        int B = tmp.at<cv::Vec3b>(y, x)[2];

        Img_copy = QImage((const uchar *) (tmp_copy.data), tmp_copy.cols, tmp_copy.rows,
                          tmp_copy.cols * frame.channels(), QImage::Format_RGB888);
        ui->frame->setPixmap(QPixmap::fromImage(Img_copy));
        ui->statusbar->showMessage(
                "当前鼠标点的像素值为：rgb(" + QString::number(R) + "," + QString::number(G) + "," +
                QString::number(B) + ")");
    } else {
        ui->frame->setPixmap(QPixmap::fromImage(Img));
    }
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    m_cameraRecord = new CameraRecord(640, 640, 30.0);

    qRegisterMetaType<cv::Mat>("cv::Mat");

    RefreshUVCDeviceList();

    connect(&m_cameraCore, SIGNAL(SendFrame(cv::Mat)), this, SLOT(Display(cv::Mat)));
    connect(this, SIGNAL(SendStatue(bool)), &m_cameraCore, SLOT(SetPreviewStatue(bool)));
    connect(this, SIGNAL(SetRecordStatue(bool)), &m_cameraCore, SLOT(_SetRecordStatue(bool)));
    connect(&m_cameraCore, SIGNAL(SendFrame2Record(cv::Mat)), m_cameraRecord, SLOT(GetFrame(cv::Mat)));
    connect(this, SIGNAL(SetRotateStatue(bool)), &m_cameraCore, SLOT(SetRotateFrame(bool)));

    connect(ui->frame, &MouseTrackLabel::clicked, this, [=](QMouseEvent *ev) {
        QPoint tmp = ev->pos();
        m_X = tmp.x();
        m_Y = tmp.y();
    });

    connect(ui->do_uvc_refresh, &QPushButton::clicked, this, [=]() {
        RefreshUVCDeviceList();
    });
    connect(ui->uvc_list, &QListWidget::itemClicked, this, [=]() {
        UVCDeviceDetail();
    });
    connect(ui->open_btn, &QPushButton::clicked, this, [=]() {
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
    connect(ui->start_btn, &QPushButton::clicked, this, [=]() {
        emit SetRecordStatue(true);
        m_cameraRecord->start();
        ui->start_btn->setEnabled(false);
        ui->pause_btn->setEnabled(true);
        ui->stop_btn->setEnabled(true);
        ui->rotate_btn->setEnabled(true);
        ui->close_btn->setEnabled(false);
    });
    connect(ui->pause_btn, &QPushButton::clicked, this, [=](){
        if (ui->pause_btn->text() == "继续") {
            ui->pause_btn->setText("暂停");
            m_cameraRecord->PauseRecord(false);
        } else {
            ui->pause_btn->setText("继续");
            m_cameraRecord->PauseRecord(true);
        }
    });
    connect(ui->stop_btn, &QPushButton::clicked, this, [=](){
        emit SetRecordStatue(false);
        m_cameraRecord->StopRecord(false);
        ui->start_btn->setEnabled(true);
        ui->stop_btn->setEnabled(false);
        ui->pause_btn->setEnabled(false);
        ui->close_btn->setEnabled(true);

    });
    connect(ui->rotate_btn, &QPushButton::clicked, this, [=]() {
        if (ui->rotate_btn->isChecked())
                emit SetRotateStatue(true);
        else
                emit SetRotateStatue(false);
    });

}

void MainWindow::UVCDeviceDetail() {
    QListWidgetItem *item = ui->uvc_list->currentItem();
    m_curUVCDevice = m_uvcDevices[item->text()];
    LOG::logger(LOG::LogLevel::INFO, "User selected device: " + item->text(), true);

    ui->uvc_des->setText(m_curUVCDevice.des);
    ui->p_1->setText(m_curUVCDevice.p1);
    ui->p_2->setText(m_curUVCDevice.p2);
    ui->p_3->setText(m_curUVCDevice.p3);
    ui->p_4->setText(m_curUVCDevice.p4);
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