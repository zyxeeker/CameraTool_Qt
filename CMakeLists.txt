cmake_minimum_required(VERSION 3.5)

project(Camera_qt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenCV_DIR ${CMAKE_SOURCE_DIR}/3rd/opencv451)
set(D_SHOW_INCLUDE ${CMAKE_SOURCE_DIR}/3rd/DirectShow)

include_directories(${D_SHOW_INCLUDE})

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

find_package(Qt5Network REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(OpenCV REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        service/camera_detect.h
        service/camera_detect.cpp
        param.h
        logger/logger.cpp
        logger/logger.h
        service/camera_core.cpp
        service/camera_core.h
        service/camera_record.cpp
        service/camera_record.h
        component/mouse_track_label.cpp
        component/mouse_track_label.h
        service/UVC_dshow.cpp
        service/UVC_dshow.h
        )


add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
        image.qrc
        )

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC Qt${QT_VERSION_MAJOR}::Multimedia)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${OpenCV_LIBS})
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC strmiids.lib)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    file(GLOB_RECURSE DEPEND_DLLS
            ${OpenCV_DIR}/x64/vc15/bin/opencv_world451d.dll
            ${OpenCV_DIR}/x64/vc15/bin/opencv_videoio_ffmpeg451_64d.dll
            )
else ()
    file(GLOB_RECURSE DEPEND_DLLS
            ${OpenCV_DIR}/x64/vc15/bin/opencv_world451.dll
            ${OpenCV_DIR}/x64/vc15/bin/opencv_videoio_ffmpeg451_64.dll
            )
endif ()
file(COPY ${DEPEND_DLLS} DESTINATION .)

