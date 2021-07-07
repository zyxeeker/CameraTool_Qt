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

    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 680);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 680);

//    cap.set(cv::CAP_PROP_FORMAT,-1);

    if (!cap.isOpened())
        std::cout << "could not open the VideoCapture !" << std::endl;

    const char *windowsName = "Example";

    int frame_1 = 200;
    int border_v = 0;
    int border_h = 0;

//    CameraRecord recorder(cap.get(3), cap.get(4), 30.0);
    CameraRecord recorder(680, 680, 30.0);
    recorder.StartRecord();

    while (frame_1 != 0) {
        cv::Mat frame;
        border_v = 0;
        border_h = 0;

        bool statue = cap.read(frame);
        if (!statue) break;

//        emit ;
        if (frame_1<=100)
            cv::transpose(frame, frame);

        double tmp_3 = double(frame.cols)/double(frame.rows);

        if (tmp_3>=1)
            border_v = (tmp_3*680-680)/2;
        else
            border_h = ((double(frame.rows)/double(frame.cols))*680-680)/2;

        cv::copyMakeBorder(frame, frame, border_v, border_v, border_h, border_h, cv::BORDER_CONSTANT, 0);
        cv::resize(frame,frame,cv::Size(680,680));

        recorder.Recorder(frame);

        cv::imshow(windowsName, frame);

        cv::waitKey(10);
        std::cout << frame_1 << std::endl;
        --frame_1;
    }

    recorder.StopRecord();
    cap.release();
}
