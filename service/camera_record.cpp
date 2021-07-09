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

void CameraRecord::run() {
    InitialCore();
    while (t1) {
        if (!m_frame.empty()) {
            if (!m_pause)
                m_vOut << m_frame;
        }
    }
    m_vOut.release();
}