//
// Created by zyx on 2021/7/16.
//

#ifndef TEST_UVC_DSHOW_H
#define TEST_UVC_DSHOW_H
#define CAMERA_GAIN 0

#include <windows.h>
#include <iostream>
#include <dshow.h>
#include <ks.h>
#include <Ksmedia.h>
#include <map>
#include "logger/logger.h"

class UVCCameraLibrary {
public:
    UVCCameraLibrary();

    ~UVCCameraLibrary();

    static void ListDevices(char **devices, int &nDevices);

    bool ConnectDevice(char *deviceName);

    void DisconnectDevice();

    HRESULT GetExposure();

    HRESULT SetExposure(int Val);

    std::map<std::string, int> get_exposure_vals() { return m_exposure_vals; }

    // Powerline Frequency
    HRESULT SetPowerLineFrequency(u_long freq_type);

    HRESULT GetPowerLineFrequency();

    u_long get_power_line_freq() { return m_power_line_freq; }

#if CAMERA_GAIN
    HRESULT SetGain(int val);
    HRESULT GetGain();
#endif
private:
    std::map<std::string, int> m_exposure_vals;
    IBaseFilter *m_DeviceFilter = nullptr;
    ICreateDevEnum *m_CreateDevEnum = nullptr;
    IEnumMoniker *m_EnumMoniker = nullptr;
    IMoniker *m_Moniker = nullptr;
    ULONG m_Fetched = 0;
    IAMCameraControl *m_camera_control = 0;
    IAMVideoProcAmp *m_video_proc = 0;
    u_long m_power_line_freq = 0;
private:
    void _GetEnumMoniker();
};

#endif //TEST_UVC_DSHOW_H
