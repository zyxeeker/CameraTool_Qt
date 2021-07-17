//
// Created by zyx on 2021/7/16.
//

#include "UVC_dshow.h"

UVCCameraLibrary::UVCCameraLibrary() {
    // initialize COM
    CoInitialize(0);
}

UVCCameraLibrary::~UVCCameraLibrary() {
    if (m_EnumMoniker != nullptr)
        m_EnumMoniker->Release();
    if (m_CreateDevEnum != nullptr)
        m_CreateDevEnum->Release();
    CoUninitialize();
}

void UVCCameraLibrary::ListDevices(char **cameraNames, int &nDevices) {
    nDevices = 0;
    IBaseFilter *pDeviceFilter = nullptr;
    ICreateDevEnum *pCreateDevEnum = nullptr;
    IEnumMoniker *pEnumMoniker = nullptr;
    IMoniker *pMoniker = nullptr;
    ULONG nFetched = 0;
    // initialize COM
    CoInitialize(0);
    // Create CreateDevEnum to list device
    CoCreateInstance(CLSID_SystemDeviceEnum, nullptr, CLSCTX_INPROC_SERVER,
                     IID_ICreateDevEnum, (PVOID *) &pCreateDevEnum);

    pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,
                                          &pEnumMoniker, 0);
    if (pEnumMoniker == nullptr) {
        std::cout << "no device" << std::endl;
        return;
    }
    pEnumMoniker->Reset();

    while (pEnumMoniker->Next(1, &pMoniker, &nFetched) == S_OK) {
        char cameraRealNames[256][256];
        IPropertyBag *pPropertyBag;
        TCHAR devname[256];
        pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void **) &pPropertyBag);
        VARIANT var;
        var.vt = VT_BSTR;
        pPropertyBag->Read(L"FriendlyName", &var, 0);//DevicePath
        WideCharToMultiByte(CP_ACP, 0, var.bstrVal, -1, devname, sizeof(devname), 0, 0);
        VariantClear(&var);

        int nSameNamedDevices = 0;
        for (int j = 0; j < nDevices; j++) {
            if (strcmp(cameraRealNames[j], devname) == 0)
                nSameNamedDevices++;
        }
        strcpy_s(cameraRealNames[nDevices], devname);
        if (nSameNamedDevices > 0)
            std::cout << "Camera name: " << devname << std::endl;
        strcpy_s(cameraNames[nDevices], sizeof(devname), (TCHAR *) devname);

        pMoniker->Release();
        pPropertyBag->Release();
        nDevices++;
    }
    pEnumMoniker->Release();
    pCreateDevEnum->Release();

    CoUninitialize();
}

void UVCCameraLibrary::_GetEnumMoniker() {
    CoCreateInstance(CLSID_SystemDeviceEnum, nullptr, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum,
                     (PVOID *) &m_CreateDevEnum);

    m_CreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &m_EnumMoniker, 0);
    if (m_EnumMoniker == nullptr) {
        std::cout << "No device" << std::endl;
        return;
    }
    // reset EnumMoniker
    m_EnumMoniker->Reset();
}

bool UVCCameraLibrary::ConnectDevice(char *deviceName) {
    _GetEnumMoniker();

    if (m_EnumMoniker == nullptr)
        return false;

    int nDevices = 0;
    while (m_EnumMoniker->Next(1, &m_Moniker, &m_Fetched) == S_OK) {
        char cameraRealNames[256][256];

        IPropertyBag *pPropertyBag;
        TCHAR devname[256];
        m_Moniker->BindToStorage(0, 0, IID_IPropertyBag,
                                 (void **) &pPropertyBag);
        VARIANT var;
        var.vt = VT_BSTR;
        pPropertyBag->Read(L"FriendlyName", &var, 0);
        WideCharToMultiByte(CP_ACP, 0,
                            var.bstrVal, -1, devname, sizeof(devname), 0, 0);
        VariantClear(&var);

        int nSameNamedDevices = 0;
        for (int j = 0; j < nDevices; j++) {
            if (strcmp(cameraRealNames[j], devname) == 0)
                nSameNamedDevices++;
        }
        strcpy_s(cameraRealNames[nDevices], devname);

        if (nSameNamedDevices > 0)
            std::cout << "Camera name: " << devname << std::endl;
        if (strcmp(devname, deviceName) == 0) {
            m_Moniker->BindToObject(0, 0, IID_IBaseFilter,
                                    (void **) &m_DeviceFilter);
            if (m_DeviceFilter != nullptr)
                return true;
        }

        // release
        m_Moniker->Release();
        pPropertyBag->Release();

        nDevices++;
    }
    return false;
}

void UVCCameraLibrary::DisconnectDevice() {
    if (m_DeviceFilter != nullptr)
        m_DeviceFilter->Release();
    m_DeviceFilter = nullptr;
}

HRESULT UVCCameraLibrary::GetExposure() {
    HRESULT hr;
    IAMCameraControl *pCameraControl = 0;
    hr = m_DeviceFilter->QueryInterface(IID_IAMCameraControl, (void **) &pCameraControl);
    if (FAILED(hr))
        std::cout << "This device does not support IAMCameraControl" << std::endl;
    else {
        long Min, Max, Step, Default, Flags, Val;
        hr = pCameraControl->GetRange(CameraControl_Exposure, &Min, &Max, &Step, &Default, &Flags);
        hr = pCameraControl->Get(CameraControl_Exposure, &Val, &Flags);
        std::cout << "Min:" << Min << ";Max:" << Max << ";Step:" << Step << std::endl;
        std::cout << "Val:" << Val << std::endl;
        if (SUCCEEDED(hr)) {
            m_vals["max"] = Max;
            m_vals["cur"] = Val;
            m_vals["min"] = Min;
        } else {
            std::cout << "This device does not support PTZControl" << std::endl;
        }
    }
    if (pCameraControl != nullptr)
        pCameraControl->Release();
    return hr;
}

HRESULT UVCCameraLibrary::SetExposure(int Val) {
    HRESULT hr;
    IAMCameraControl *pCameraControl = 0;
    hr = m_DeviceFilter->QueryInterface(IID_IAMCameraControl, (void **) &pCameraControl);
    if (FAILED(hr))
        std::cout << "This device does not support IAMCameraControl" << std::endl;

    else
        hr = pCameraControl->Set(CameraControl_Exposure, Val, CameraControl_Flags_Manual);
    if (pCameraControl != nullptr)
        pCameraControl->Release();
    return hr;
}

#if CAMERA_GAIN
HRESULT UVCCameraLibrary::GetGain() {
    HRESULT hr;
    IAMVideoProcAmp *pProcAmp = 0;
    hr = m_DeviceFilter->QueryInterface(IID_IAMVideoProcAmp, (void **) &pProcAmp);
    if (FAILED(hr)) {
        std::cout<<"This device does not support AMVideoProcAmp"<<std::endl;
    } else {
        long Min, Max, Step, Default, Flags, Val;
        hr = pProcAmp->GetRange(VideoProcAmp_Gain, &Min, &Max, &Step,&Default, &Flags);
        if (SUCCEEDED(hr)) {
            hr = pProcAmp->Get(VideoProcAmp_Brightness, &Val, &Flags);
        }
        if (SUCCEEDED(hr)) {
            std::cout << "Max:" << Max << " Min:" << Min << " Val:" << Val << std::endl;
        }
        else {
            std::cout<<"This device does not support AMVideoProcAmp"<<std::endl;
        }

    }
    return hr;
}

HRESULT UVCCameraLibrary::SetGain(int Val)
{
    HRESULT hr;
    IAMVideoProcAmp *pProcAmp = 0;
    hr = m_DeviceFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&pProcAmp);
    if (FAILED(hr))
        std::cout<<"This device does not support AMVideoProcAmp"<<std::endl;
    else
        hr = pProcAmp->Set(VideoProcAmp_Gain,Val,VideoProcAmp_Flags_Manual);
    if (pProcAmp != nullptr)
        pProcAmp->Release();
    return hr;
}
#endif