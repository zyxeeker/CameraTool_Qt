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
#include "service/UVC_dshow.h"

#define FRAME_TEST 0
#define USE_OPENCV 0
#define USE_DSHOW 1

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
    UVCCameraLibrary m_lib;
    QMap<QString, Dev::UVCDevice> m_uvcDevices;
    QString m_selectedDeivce;
    Dev::UVCDevice m_curUVCDevice;
    int m_X = -1;
    int m_Y = -1;
#if FRAME_TEST
    double m_XTmp = 0;
    double m_YTmp = 0;
#endif
    int m_LastX = -1;
    int m_LastY = -1;

private slots:

    void Display(cv::Mat frame);

    void on_exposure_value_valueChanged();

signals:

    void SendStatue(bool statue);

    void SetRecordStatue(bool mark);

    void SetRotateStatue(bool rt);
};
#endif // MAINWINDOW_H
