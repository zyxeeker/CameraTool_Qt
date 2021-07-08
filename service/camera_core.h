//
// Created by zyx on 2021/7/5.
//

#ifndef TEST_CAMERA_CORE_H
#define TEST_CAMERA_CORE_H

#include <opencv2/opencv.hpp>
#include <QThread>
#include <string>

//TODO 初始化core作为private函数
//TODO 加入QT线程防止UI卡死
//TODO 加入信号槽连接UI界面

class CameraRecord : public QThread{
    Q_OBJECT
public:
    CameraRecord(double capHeight, double capWidth, double fps) :
            m_capHeight(capHeight), m_capWidth(capWidth), m_fps(fps) {}

    bool InitialCore();

    void StartRecord();

    void PauseRecord();

    void ResumeRecord();

    void StopRecord();

    void Recorder(cv::Mat frame);

private:
    cv::VideoWriter m_vOut;

    std::string m_filePath;
    double m_fps;
    double m_capHeight;
    double m_capWidth;

    bool m_recordStatue = true;
};

class CameraCore : public QThread {
    Q_OBJECT
public:
    ~CameraCore() { m_cap.release(); }
    bool OpenCamera();

    void CameraPreview();
protected:
    void run() override;
private:
    cv::VideoCapture m_cap;
    bool m_previewStatue = true;
signals:
    bool SendStatue();
    void SendFrame(cv::Mat frame);
//private slots:
//    bool SetPreviewStatue(bool statue);
};


#endif //TEST_CAMERA_CORE_H
