//
// Created by zyx on 2021/7/5.
//

#include "camera_core.h"
#include <iostream>


bool CameraRecord::InitialCore() {
    m_vOut.open("test.avi", CV_FOURCC('M', 'J', 'P', 'G'), m_fps, cv::Size(m_capHeight, m_capWidth), true);
    if (!m_vOut.isOpened()) {
        std::cout << "Recorder Fail!" << std::endl;
        return false;
    }
    return true;
}

void CameraRecord::Recorder(cv::Mat frame) {
//    while (m_recordStatue)
    m_vOut << frame;
}

void CameraRecord::StartRecord() {
    if (!InitialCore())
        std::cout << "ERROR!RECORDER!" << std::endl;

}

void CameraRecord::PauseRecord() {
    m_recordStatue = false;
}

void CameraRecord::ResumeRecord() {
    m_recordStatue = true;
}

void CameraRecord::StopRecord() {
    m_recordStatue = false;
    m_vOut.release();
}

void CameraCore::OpenCamera_test() {
    cv::VideoCapture cap;
    cap.open(0, cv::CAP_DSHOW);

    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 680);

//    cap.set(cv::CAP_PROP_FORMAT,-1);

    if (!cap.isOpened())
        std::cout << "could not open the VideoCapture !" << std::endl;

    const char *windowsName = "Example";

    int frame_1 = 100;

    CameraRecord recorder(cap.get(4), cap.get(3), 30.0);
    recorder.StartRecord();

    while (frame_1 != 0) {
        cv::Mat frame;

        bool statue = cap.read(frame);
        if (!statue) break;

//        emit ;
//        cv::transpose(frame, frame);

        recorder.Recorder(frame);

        imshow(windowsName, frame);    //从视频对象中获取图片显示到窗口

        cv::waitKey(10);    //每33毫秒一张图片
        std::cout << frame_1 << std::endl;
        --frame_1;
    }

    recorder.StopRecord();
    cap.release();
}
