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
    ~CameraCore() {
#if 1
        std::cout<<"Thread Destroyed!"<<std::endl;
#endif
        m_cap.release(); }

    bool OpenCamera();

    void CameraPreview();

    bool GetCurMark() { return m_curMark; }

protected:
    void run() override;
private:
    bool m_curMark = false;
    cv::VideoCapture m_cap;
    bool m_previewStatue = true;

signals:
    void SendFrame(cv::Mat frame);
    void SendFrame2Record(cv::Mat frame);

private slots:
    void SetPreviewStatue(bool statue) { m_previewStatue = statue; }
    void _SetRecordStatue(bool st) { m_curMark = st; }
};


#endif //TEST_CAMERA_CORE_H
