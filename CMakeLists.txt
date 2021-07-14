cmake_minimum_required(VERSION 3.19)
project(QtTemplateMatching)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets)
set(suff ${CMAKE_SHARED_LIBRARY_SUFFIX})

add_executable(${PROJECT_NAME} main.cpp mainwindow.cpp mainwindow.h mainwindow.ui matchingmanager.cpp matchingmanager.h resizableimagelabel.cpp resizableimagelabel.h)

if (NOT CMAKE_PREFIX_PATH)
    message(FATAL_ERROR "CMAKE_PREFIX_PATH (path to Qt Cmake) is needed to compile this project.")
endif()

if (NOT OPENCV_INCLUDE_PATH)
    message(FATAL_ERROR "OPENCV_INCLUDE_PATH is not defined, you need it to compile this project.")
endif()

if (NOT OPENCV_LIB_PATH)
    message(FATAL_ERROR "OPENCV_LIB_PATH is not defined, you need it to compile this project.")
endif()

include_directories(${OPENCV_INCLUDE_PATH})

target_link_libraries(${PROJECT_NAME} ${OPENCV_LIB_PATH}/libopencv_core${suff}
        ${OPENCV_LIB_PATH}/libopencv_imgcodecs${suff} ${OPENCV_LIB_PATH}/libopencv_highgui${suff}
        ${OPENCV_LIB_PATH}/libopencv_imgproc${suff})

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})



