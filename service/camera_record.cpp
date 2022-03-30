//
// Created by zyx on 2021/7/9.
//

#include <logger/logger.h>
#include "camera_record.h"

bool CameraRecord::InitialCore() {
    m_vOut.open("test.avi", cv::CAP_OPENCV_MJPEG, m_fps, cv::Size(m_capWidth, m_capHeight), true);
    if (!m_vOut.isOpened()) {
        LOG::logger(LOG::LogLevel::FATAL, "Recorder fail to open file!");
        return false;
    }
    LOG::logger(LOG::LogLevel::INFO, "Recorder success to open file!");
    return true;
}

cv::Mat CameraRecord::HandleFrame(cv::Mat frame) {
    int border_v = 0;
    int border_h = 0;

    double VofH = double(frame.cols) / double(frame.rows);

    if (VofH >= 1)
        border_v = (VofH * m_capHeight - m_capWidth) / 2;
    else
        border_h = ((double(frame.rows) / double(frame.cols)) * m_capWidth - m_capHeight) / 2;

    cv::copyMakeBorder(frame, frame, border_v, border_v, border_h, border_h, cv::BORDER_CONSTANT, 0);
    cv::resize(frame, frame, cv::Size(m_capWidth, m_capHeight));
    return frame;
}

void CameraRecord::run() {
    m_statue = true;
    InitialCore();
    while (m_statue) {
        if (!m_frame.empty()) {
            if (!m_pause) {
                cv::Mat frame = HandleFrame(m_frame);
                m_vOut << frame;
                cv::waitKey(30);
            }
        }
    }
    m_vOut.release();
    LOG::logger(LOG::LogLevel::INFO, "Saved file!");
}
