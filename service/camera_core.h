//
// Created by zyx on 2021/7/5.
//

#ifndef TEST_CAMERA_CORE_H
#define TEST_CAMERA_CORE_H

#include <opencv2/opencv.hpp>
#include <QThread>
#include <string>

class CameraCore : public QThread {
Q_OBJECT
public:
    ~CameraCore() { m_cap.release(); }

    bool OpenCamera();

    void CameraPreview();

    bool GetCurMark() { return m_curMark; }

    void SetExposure(int ev) { m_exposureValue = ev; };

protected:
    void run() override;

private:
    int m_exposureValue = 0;
    bool m_curMark = false;
    cv::VideoCapture m_cap;
    bool m_previewStatue = true;
    bool m_rotateStatue = false;

signals:
    void SendFrame(cv::Mat frame);
    void SendFrame2Record(cv::Mat frame);

private slots:
    void SetPreviewStatue(bool statue) { m_previewStatue = statue; }

    void _SetRecordStatue(bool st) { m_curMark = st; }

    void SetRotateFrame(bool mark) { m_rotateStatue = mark; }
};


#endif //TEST_CAMERA_CORE_H
