#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAddressEntry>
#include <QCloseEvent>
#include "service/camera_detect.h"
#include "service/camera_core.h"
#include "service/camera_record.h"
#include "param.h"
#include "logger/logger.h"
#include "opencv2/opencv.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void RefreshUVCDeviceList();

    void UVCDeviceDetail();

    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    CameraDetect m_detectService;
    CameraCore m_cameraCore;
    CameraRecord *m_cameraRecord;
    QMap<QString, Dev::UVCDevice> m_uvcDevices;

    Dev::UVCDevice m_curUVCDevice;
    int mX = -1;
    int mY = -1;

private slots:

    void Display(cv::Mat frame);

signals:

    void SendStatue(bool statue);

    void SetRecordStatue(bool mark);

    void SetRotateStatue(bool rt);
};
#endif // MAINWINDOW_H
