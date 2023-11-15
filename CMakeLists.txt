cmake_minimum_required(VERSION 3.5)

project(FRAM VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS .)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # Force MSVC to use UTF-8 because that's what we use. Otherwise it uses
    # the default of whatever Windows sets and causes encoding issues.
    message(STATUS "You are using MSVC. Forcing to use UTF-8")
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/wd4828>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/wd4828>")
endif ()

set(DEPENDS "")
# add_qt
#set(QT_VERSION "6" CACHE STRING "Qt version")
#set(OpenCV_DIR "E:/develop/opencv/x64/vc16/staticlib" CACHE PATH "fastdeploy install directory")
#set(SEETAFACE_DIR "/home/orangepi/my/seeta-face6/build/" CACHE PATH "seetaface6 install directory")
#set(QxOrm_DIR "/home/orangepi/my/QxOrm/build/install/" CACHE PATH "QxOrm install directory")
#set(CMAKE_TOOLCHAIN_FILE "E:/vcpkg/scripts/buildsystems/vcpkg.cmake")


set(QT_VERSION "6" CACHE STRING "Qt version")
set(OpenCV_DIR "E:/develop/opencv/x64/vc16/staticlib" CACHE PATH "opencv install directory")
set(SEETAFACE_DIR "E:/develop/SeetaFace" CACHE PATH "seetaface6 install directory")
set(QxOrm_DIR "E:/develop/QxOrm" CACHE PATH "QxOrm install directory")
set(CMAKE_TOOLCHAIN_FILE "E:/vcpkg/scripts/buildsystems/vcpkg.cmake")

find_package(Qt${QT_VERSION} COMPONENTS Widgets Sql Concurrent Network REQUIRED)
list(APPEND DEPENDS Qt${QT_VERSION}::Widgets Qt${QT_VERSION}::Sql Qt${QT_VERSION}::Concurrent Qt${QT_VERSION}::Network)

#add opencv
find_package(OpenCV REQUIRED)
list(APPEND DEPENDS ${OpenCV_LIBS})

#add faiss
find_package(OpenMP REQUIRED)
list(APPEND DEPENDS OpenMP::OpenMP_CXX)

find_package(faiss REQUIRED)
list(APPEND DEPENDS faiss)

# add seetaface
if (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "AMD64" OR "x68_64")
    set(ARCH "x64")
elseif (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "aarch64")
    set(ARCH "arm64")
else ()
    message(FATAL "cpu arch x86 and aarch64 only be surrorted")
endif ()
message(${SEETAFACE_DIR}/lib/${ARCH})
include_directories(${SEETAFACE_DIR}/include)
link_directories(${SEETAFACE_DIR}/lib/${ARCH})
list(APPEND DEPENDS
        SeetaFaceLandmarker600
        SeetaFaceRecognizer610
        SeetaFaceDetector600
        SeetaFaceTracking600
        SeetaFaceAntiSpoofingX600)

#add QxOrm

include_directories(${QxOrm_DIR}/include)
link_directories(${QxOrm_DIR}/lib)
list(APPEND DEPENDS QxOrm)
include_directories(${CMAKE_SOURCE_DIR}/widgets)

# add source code
set(PROJECT_SOURCES
        models/models.h
        models/models.cpp
        core/facerecthread.h
        core/facerecthread.cpp
        core/facedetthread.h
        core/facedetthread.cpp
        core/vectorsearch.h
        core/vectorsearch.cpp
        widgets/mywidget.h
        widgets/mywidget.cpp
        widgets/mywidget.ui
        widgets/registerwidget.h
        widgets/registerwidget.cpp
        widgets/registerwidget.ui
        widgets/historywidget.h
        widgets/historywidget.cpp
        widgets/historywidget.ui
        widgets/faceinfowidget.h
        widgets/faceinfowidget.cpp
        widgets/faceinfowidget.ui
        qrc.qrc
        core/recordthread.h
        core/recordthread.cpp
        core/SeetaFace.h
        core/SeetaFace.cpp
        utils/struct.h
        utils/Utils.h
        utils/Utils.cpp
        utils/snowflake.hpp
        utils/config.h
        utils/config.cpp
        customwidget/pagingwidget.h
        customwidget/pagingwidget.cpp
        customwidget/pagingwidget.ui
        main.cpp
        core/outersocket.cpp
        core/outersocket.h
        widgets/MySplashScreen.cpp
        widgets/MySplashScreen.h
        widgets/attendinfowidget.h
        widgets/attendinfowidget.cpp
        widgets/attendinfowidget.ui
        widgets/inputpage.h
        widgets/inputpage.cpp
        widgets/inputpage.ui
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} ${DEPENDS})
