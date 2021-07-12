//
// Created by zyx on 2021/7/9.
//

#include "camera_record.h"

bool CameraRecord::InitialCore() {
    t1 = true;
    m_vOut.open("test.avi", CV_FOURCC('M', 'J', 'P', 'G'), m_fps, cv::Size(m_capWidth, m_capHeight), true);
    if (!m_vOut.isOpened()) {
        std::cout << "Recorder Fail!" << std::endl;
        return false;
    }
    return true;
}

//void CameraRecord::PauseRecord() {
//}
//
//void CameraRecord::ResumeRecord() {
//}

cv::Mat CameraRecord::HandleFrame(cv::Mat frame) {
    int border_v = 0;
    int border_h = 0;

    double VofH = double(frame.cols) / double(frame.rows);

    if (VofH >= 1)
        border_v = (VofH * m_capHeight - m_capWidth) / 2;
    else
        border_h = ((double(frame.rows) / double(frame.cols)) * 680 - 680) / 2;

    cv::copyMakeBorder(frame, frame, border_v, border_v, border_h, border_h, cv::BORDER_CONSTANT, 0);
    cv::resize(frame, frame, cv::Size(m_capWidth, m_capHeight));
    return frame;
}


void CameraRecord::run() {
    InitialCore();
    while (t1) {
        if (!m_frame.empty()) {
            if (!m_pause) {
                cv::Mat frame = HandleFrame(m_frame);
                m_vOut << frame;
                cv::waitKey(30);
            }
        }
    }
    m_vOut.release();
}