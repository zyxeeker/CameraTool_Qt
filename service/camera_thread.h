//
// Created by zyx on 2021/7/6.
//

#ifndef TEST_CAMERA_THREAD_H
#define TEST_CAMERA_THREAD_H

#include <QThread>
#include <opencv2/opencv.hpp>
#include "camera_core.h"

class CameraThread : public QThread{
Q_OBJECT
public:
//    void run() override {}
signals:
//    void CameraPreview(cv::Mat frame);

private:
    CameraCore m_cameraCore;
};


#endif //TEST_CAMERA_THREAD_H
