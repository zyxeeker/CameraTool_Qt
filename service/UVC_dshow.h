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

class UVCCameraLibrary {
public:
    UVCCameraLibrary();

    ~UVCCameraLibrary();

    static void ListDevices(char **devices, int &nDevices);

    bool ConnectDevice(char *deviceName);

    void DisconnectDevice();

    HRESULT GetExposure();

    HRESULT SetExposure(int Val);

    std::map<std::string, int> GetVal() { return m_vals; }

#if CAMERA_GAIN
    HRESULT SetGain(int val);
    HRESULT GetGain();
#endif
private:
    std::map<std::string, int> m_vals;
    IBaseFilter *m_DeviceFilter = nullptr;
    ICreateDevEnum *m_CreateDevEnum = nullptr;
    IEnumMoniker *m_EnumMoniker = nullptr;
    IMoniker *m_Moniker = nullptr;
    ULONG m_Fetched = 0;
private:
    void _GetEnumMoniker();
};

#endif //TEST_UVC_DSHOW_H
