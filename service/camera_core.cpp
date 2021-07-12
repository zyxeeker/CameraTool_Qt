//
// Created by zyx on 2021/7/5.
//

#include "camera_core.h"
#include <iostream>
#define TEST_DEBUG 0

bool CameraCore::OpenCamera() {
    m_cap.open(0, cv::CAP_DSHOW);
    if (!m_cap.isOpened()) {
        std::cout << "could not open the VideoCapture !" << std::endl;
        return false;
    }

    m_cap.set(cv::CAP_PROP_FRAME_HEIGHT, 680);
    m_cap.set(cv::CAP_PROP_FRAME_WIDTH, 680);

//    cap.set(cv::CAP_PROP_FORMAT,-1);


    const char *windowsName = "Example";

    int frame_1 = 200;
#if TEST_DEBUG
    int border_v = 0;
    int border_h = 0;
#endif
#if TEST_DEBUG
    //    CameraRecord recorder(cap.get(3), cap.get(4), 30.0);
    CameraRecord recorder(680, 680, 30.0);
    recorder.StartRecord();

    while (frame_1 != 0) {
        cv::Mat frame;
        m_cap.read(frame);
        if (!statue) break;
#endif
#if TEST_DEBUG
        border_v = 0;
        border_h = 0;
        if (frame_1<=100)
            cv::transpose(frame, frame);

        double tmp_3 = double(frame.cols)/double(frame.rows);

        if (tmp_3>=1)
            border_v = (tmp_3*680-680)/2;
        else
            border_h = ((double(frame.rows)/double(frame.cols))*680-680)/2;

        cv::copyMakeBorder(frame, frame, border_v, border_v, border_h, border_h, cv::BORDER_CONSTANT, 0);
        cv::resize(frame,frame,cv::Size(680,680));


//        recorder.Recorder(frame);

        cv::imshow(windowsName, frame);

        cv::waitKey(10);
        std::cout << frame_1 << std::endl;
        --frame_1;
    }

//    recorder.StopRecord();
#endif
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
    }
}