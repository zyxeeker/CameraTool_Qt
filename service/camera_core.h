//
// Created by zyx on 2021/7/5.
//

#ifndef TEST_CAMERA_CORE_H
#define TEST_CAMERA_CORE_H

#include <opencv2/opencv.hpp>
#include <QThread>
#include <string>

class CameraRecord {
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
    void OpenCamera_test();
//    void SetPreviewStatueStart() { m_previewStatue = true; }
//    void SetPreviewStatueStop() { m_previewStatue = false; }
private:
    bool m_previewStatue = true;
};


#endif //TEST_CAMERA_CORE_H
