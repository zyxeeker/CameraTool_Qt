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
    if (m_camera_control != nullptr)
        m_camera_control->Release();
    if (m_video_proc != nullptr)
        m_video_proc->Release();

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
            LOG::logger(LOG::LogLevel::INFO, "Camera name:" + std::string(devname));
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
            LOG::logger(LOG::LogLevel::INFO, "Camera name:" + std::string(devname));
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
    if(m_camera_control == 0){
        hr = m_DeviceFilter->QueryInterface(IID_IAMCameraControl, (void **) &m_camera_control);
        if (FAILED(hr)){
            LOG::logger(LOG::LogLevel::WARN, "This device does not support IAMCameraControl");
            return hr;
        }
    }

    long Min, Max, Step, Default, Flags, Val;
    hr = m_camera_control->GetRange(CameraControl_Exposure, &Min, &Max, &Step, &Default, &Flags);
    hr = m_camera_control->Get(CameraControl_Exposure, &Val, &Flags);
    LOG::logger(LOG::LogLevel::INFO, "Exposure: Min:" + std::to_string(Min) + ";Max:" + std::to_string(Max) + ";Step:" + std::to_string(Step));
    LOG::logger(LOG::LogLevel::INFO, "Exposure: Current Val:" + std::to_string(Val));
    if (SUCCEEDED(hr)) {
        m_exposure_vals["max"] = Max;
        m_exposure_vals["cur"] = Val;
        m_exposure_vals["min"] = Min;
    } else {
        LOG::logger(LOG::LogLevel::WARN, "This device does not support PTZControl");
    }

    return hr;
}

HRESULT UVCCameraLibrary::SetExposure(int Val) {
    HRESULT hr;
    if(m_camera_control == 0){
        LOG::logger(LOG::LogLevel::WARN, "Please get exposure first!");
        return E_ABORT;
    }
    hr = m_camera_control->Set(CameraControl_Exposure, Val, CameraControl_Flags_Manual);
    return hr;
}

// Powerline Frequency
HRESULT UVCCameraLibrary::GetPowerLineFrequency(){
    HRESULT hr;
    if(m_video_proc == 0){
        hr = m_DeviceFilter->QueryInterface(IID_IAMVideoProcAmp, (void **) &m_video_proc);
        if (FAILED(hr)){
            LOG::logger(LOG::LogLevel::WARN, "This device does not support IAMVideoProcAmp");
            return hr;
        }
    }
    long Flags, Val;
    m_video_proc->Get(KSPROPERTY_VIDEOPROCAMP_POWERLINE_FREQUENCY,&Val,&Flags);
    m_power_line_freq = Val;
    return S_OK;
}

HRESULT UVCCameraLibrary::SetPowerLineFrequency(u_long freq_type){
    HRESULT hr;
    if(m_camera_control == 0){
        LOG::logger(LOG::LogLevel::WARN, "Please get powerline frequency first!");
        return E_ABORT;
    }
    hr = m_video_proc->Set(KSPROPERTY_VIDEOPROCAMP_POWERLINE_FREQUENCY, freq_type, CameraControl_Flags_Manual);
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
