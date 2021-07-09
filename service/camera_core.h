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
            m_capHeight(capHeight), m_capWidth(capWidth), m_fps(fps) {};
    ~CameraRecord() { m_vOut.release(); }

    bool InitialCore();

//    void PauseRecord();
//
//    void ResumeRecord();
    void stop() { m_vOut.release(); }
    bool t1;
protected:
    void run() override;
private:
    bool m_pause = false;

    cv::VideoWriter m_vOut;
    cv::Mat m_frame;
    std::string m_filePath;
    double m_fps;
    double m_capHeight;
    double m_capWidth;
private slots:
    void GetFrame(cv::Mat frame) { m_frame = frame; }
};



class CameraCore : public QThread {
    Q_OBJECT
public:
    ~CameraCore() {
#if 1
        std::cout<<"Thread Destroyed!"<<std::endl;
#endif
        m_cap.release(); }

    bool OpenCamera();

    void CameraPreview();

    bool GetCurMark() { return m_curMark; }

protected:
    void run() override;
private:
    bool m_curMark = false;
    cv::VideoCapture m_cap;
    bool m_previewStatue = true;
    CameraRecord *m_recordService;

signals:
    void SendFrame(cv::Mat frame);
    void SendFrame2Record(cv::Mat frame);

private slots:
    void SetPreviewStatue(bool statue) { m_previewStatue = statue; }
    void _SetRecordStatue(bool st) { m_curMark = st; }
};


#endif //TEST_CAMERA_CORE_H
