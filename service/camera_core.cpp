//
// Created by zyx on 2021/7/5.
//

#include "camera_core.h"
#include "./logger/logger.h"

bool CameraCore::OpenCamera() {
    m_cap.open(0, cv::CAP_DSHOW);
    if (!m_cap.isOpened()) {
        LOG::logger(LOG::LogLevel::WARN, "Could not open the VideoCapture!");
        return false;
    }
    m_cap.set(cv::CAP_PROP_FRAME_HEIGHT, 680);
    m_cap.set(cv::CAP_PROP_FRAME_WIDTH, 680);
    return true;
}

void CameraCore::CameraPreview() {
    cv::Mat frame;
    while(true) {
        if (m_previewStatue) {
            m_cap.read(frame);
            if (m_rotateStatue) {
                cv::transpose(frame, frame);
                cv::flip(frame, frame, 1);
            }
            emit SendFrame(frame);
            if (m_curMark)
                    emit SendFrame2Record(frame);
        }
    }
}

void CameraCore::run() {
    if (OpenCamera()) {
        CameraPreview();
        LOG::logger(LOG::LogLevel::INFO, "CameraCore service is running!");
    }
}