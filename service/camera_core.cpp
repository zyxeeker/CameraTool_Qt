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
    return true;
}

void CameraCore::CameraPreview() {
    cv::Mat frame;
    int exposure_tmp = -1;
    while(true) {
        if (m_previewStatue) {
            if (m_exposureValue != exposure_tmp) {
                m_cap.set(cv::CAP_PROP_EXPOSURE, m_exposureValue);
                exposure_tmp = m_exposureValue;
            }
            m_cap.set(cv::CAP_PROP_BRIGHTNESS, 100);
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
