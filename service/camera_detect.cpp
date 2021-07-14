#include "camera_detect.h"
#include "logger/logger.h"

void CameraDetect::find_uvc_devices() {
    QList<QCameraInfo> uvcDevices = QCameraInfo::availableCameras();
    if (!uvcDevices.isEmpty()) {
        LOG::logger(LOG::LogLevel::INFO, "Found " + QString(uvcDevices.size()) + " online devices", true);
                foreach (QCameraInfo item, uvcDevices) {
                QString detailTmp = item.deviceName();
                QStringList detail = detailTmp.split('#');

                m_uvcDevice.p1 = detail[0];
                m_uvcDevice.p2 = detail[1];
                m_uvcDevice.p3 = detail[2];
                m_uvcDevice.p4 = detail[3];
                m_uvcDevice.des = item.description();

                m_uvcDeviceList[m_uvcDevice.des] = m_uvcDevice;

                LOG::logger(LOG::LogLevel::INFO, "Online device`s detail: "
                                                 "(p1): " + m_uvcDevice.p1 +
                                                 ", (p2): " + m_uvcDevice.p2 +
                                                 ", (p3): " + m_uvcDevice.p3 +
                                                 ", (p4): " + m_uvcDevice.p4 +
                                                 ", (des): " + m_uvcDevice.des, true);
            }
    }
}
