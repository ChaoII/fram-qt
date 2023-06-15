cmake_minimum_required(VERSION 3.5)

project(FRAM VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS .)

set(DEPENDS "")
# add_qt
find_package(Qt5 COMPONENTS Widgets Sql Concurrent REQUIRED)
list(APPEND DEPENDS Qt5::Widgets Qt5::Sql Qt5::Concurrent)

#add opencv
find_package(OpenCV REQUIRED)
list(APPEND DEPENDS ${OpenCV_LIBS})

#add faiss
find_package(OpenMP REQUIRED)
list(APPEND DEPENDS OpenMP::OpenMP_CXX)

find_package(faiss REQUIRED)
list(APPEND DEPENDS faiss)

# add seetaface
set(SEETAFACE_DIR "/home/orangepi/my/seeta-face6/build/")
include_directories(${SEETAFACE_DIR}/include)
link_directories(${SEETAFACE_DIR}/lib/aarch64)
list(APPEND DEPENDS
    SeetaFaceLandmarker600
    SeetaFaceRecognizer610
    SeetaFaceDetector600
    SeetaFaceTracking600
    SeetaFaceAntiSpoofingX600)

#add QxOrm
set(QxOrm_DIR "/home/orangepi/my/QxOrm/build/install/")
include_directories(${QxOrm_DIR}/include)
link_directories(${QxOrm_DIR}/lib)
list(APPEND DEPENDS QxOrm)

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
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} ${DEPENDS})


