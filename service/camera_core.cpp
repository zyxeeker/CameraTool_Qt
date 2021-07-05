//
// Created by zyx on 2021/7/5.
//

#include "camera_core.h"
#include <iostream>

void CameraCore::OpenCamera_test() {
    cv::VideoCapture cap;
    cap.open(0);
    if (!cap.isOpened())
        std::cout << "could not open the VideoCapture !" << std::endl;

    const char *windowsName = "Example";

    while (m_previewStatue) {
        cv::Mat frame;
        bool ok = cap.read(frame);
        if (!ok)    //判断视频文件是否读取结束
            break;
//        emit ;
        imshow(windowsName, frame);    //从视频对象中获取图片显示到窗口
        cv::waitKey(33);    //每33毫秒一张图片
    }

    cv::waitKey(-1);



//    cv::VideoCapture* pCvDevice = new cv::VideoCapture;
//    pCvDevice->open(0,cv::CAP_DSHOW);
//    pCvDevice->set(cv::CAP_PROP_FORMAT,-1);

//    while(true)
//    {
//        *pCvDevice >> buffer.buffer;
//        buffer.bfid = ++this->mFrameCounter;
//        cv::imshow("video",buffer.buffer);
//    }
//    auto *tmp_test = new cv::VideoCapture;
//    cv::VideoCapture tmp_test(0);
//    if(!tmp_test.isOpened())
//    {
//        std::cout<<"video not open."<<std::endl;
//    }
////    bool succ = tmp_test.open(0,cv::CAP_DSHOW);
//
////    std::cout<<succ<<std::endl;
//    std::cout<<tmp_test.isOpened()<<std::endl;
//
//    cv::Mat frame;
//    tmp_test.read(frame);
//    tmp_test>>frame;
//return nullptr;

}
