cmake_minimum_required(VERSION 3.19)
project(QtTemplateMatching)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets)
set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/qt@5/5.15.2/lib/cmake")

add_executable(${PROJECT_NAME} main.cpp mainwindow.cpp mainwindow.h mainwindow.ui matchingmanager.cpp matchingmanager.h resizableimagelabel.cpp resizableimagelabel.h)

if (NOT CMAKE_PREFIX_PATH)
    message(WARNING "CMAKE_PREFIX_PATH is not defined, you may need to set it "
            "(-DCMAKE_PREFIX_PATH=\"path/to/Qt/lib/cmake\" or -DCMAKE_PREFIX_PATH=/usr/include/{host}/qt{version}/ on Ubuntu)")
endif ()

include_directories(/opt/homebrew/Cellar/opencv/4.5.3/include/opencv4/)
target_link_libraries(${PROJECT_NAME} /opt/homebrew/Cellar/opencv/4.5.3/lib/libopencv_core.dylib /opt/homebrew/Cellar/opencv/4.5.3/lib/libopencv_imgcodecs.dylib /opt/homebrew/Cellar/opencv/4.5.3/lib/libopencv_highgui.dylib /opt/homebrew/Cellar/opencv/4.5.3/lib/libopencv_imgproc.dylib)

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})

set(dir ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${dir})
set(LIBRARY_OUTPUT_PATH  ${dir})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${dir})
set(CMAKE_BUILD_FILES_DIRECTORY ${dir})
set(CMAKE_BUILD_DIRECTORY ${dir})
set(CMAKE_BINARY_DIR ${dir})
set(CMAKE_CACHEFILE_DIR ${dir})


