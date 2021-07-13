#include "camera_detect.h"
#include "logger/logger.h"
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")


CameraDetect::CameraDetect() {
    find_uvc_devices();
}

void CameraDetect::find_uvc_devices() {
    QList<QCameraInfo> uvcDevices = QCameraInfo::availableCameras();
    if (!uvcDevices.isEmpty()) {
                foreach (QCameraInfo item, uvcDevices) {
                QString detailTmp = item.deviceName();
                QStringList detail = detailTmp.split('#');

                m_uvcDevice.p1 = detail[0];
                m_uvcDevice.p2 = detail[1];
                m_uvcDevice.p3 = detail[2];
                m_uvcDevice.p4 = detail[3];
                m_uvcDevice.des = item.description();

                m_uvcDeviceList[m_uvcDevice.des] = m_uvcDevice;

                qDebug() << detail;
                qDebug() << item.deviceName();
                qDebug() << item.description();
            }
    }
}
