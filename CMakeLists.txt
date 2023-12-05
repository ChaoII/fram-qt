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
    add_compile_options(/utf-8 /wd4828 /wd4267)
endif ()


set(DEPENDS "")


set(QT_VERSION "6" CACHE STRING "Qt version")
set(OpenCV_DIR "E:/develop/opencv/x64/vc16/staticlib" CACHE PATH "opencv install directory")
set(SEETAFACE_DIR "E:/develop/SeetaFace" CACHE PATH "seetaface6 install directory")
set(QxOrm_DIR "E:/develop/QxOrm" CACHE PATH "QxOrm install directory")


# add qt
find_package(Qt${QT_VERSION} COMPONENTS Widgets Sql Concurrent Network Multimedia REQUIRED)
list(APPEND DEPENDS Qt${QT_VERSION}::Widgets
        Qt${QT_VERSION}::Sql
        Qt${QT_VERSION}::Concurrent
        Qt${QT_VERSION}::Network
        Qt${QT_VERSION}::Multimedia)

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
    message(FATAL "cpu arch x86 and aarch64 only be supported")
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

#add include directory
include_directories(${CMAKE_SOURCE_DIR}/widgets)
include_directories(${CMAKE_SOURCE_DIR}/widgets/commonwidgets)
include_directories(${CMAKE_SOURCE_DIR}/widgets/attendhistory)
include_directories(${CMAKE_SOURCE_DIR}/widgets/faceregister)

# add src
aux_source_directory(models MODELS_SRC)
aux_source_directory(core CORE_SRC)
aux_source_directory(widgets/faceregister REGISTER_SRC)
aux_source_directory(widgets/attendhistory ATTEND_SRC)
aux_source_directory(widgets WIDGETS_SRC)
aux_source_directory(widgets/commonwidgets CUSTOM_WIDGETS_SRC)
aux_source_directory(utils UTILS_SRC)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    aux_source_directory(utils/wavPlayer WAVE_SRC)
    list(APPEND UTILS_SRC ${WAVE_SRC})
    list(APPEND DEPENDS asound)
endif ()

# add source code
set(PROJECT_SOURCES
        ${MODELS_SRC}
        ${CORE_SRC}
        ${REGISTER_SRC}
        ${ATTEND_SRC}
        ${WIDGETS_SRC}
        ${CUSTOM_WIDGETS_SRC}
        ${UTILS_SRC}
        qrc.qrc
        main.cpp
)

set(THEME_COLOR 1f64fd)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} ${DEPENDS})