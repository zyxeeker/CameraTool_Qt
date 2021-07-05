//
// Created by zyx on 2021/7/5.
//

#ifndef TEST_CAMERA_CORE_H
#define TEST_CAMERA_CORE_H

#include <opencv2/opencv.hpp>
#include <QThread>


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
