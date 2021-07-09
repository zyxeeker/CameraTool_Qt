//
// Created by zyx on 2021/7/9.
//

#ifndef TEST_CAMERA_RECORD_H
#define TEST_CAMERA_RECORD_H

#include <opencv2/opencv.hpp>
#include <QThread>

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


#endif //TEST_CAMERA_RECORD_H
