cmake_minimum_required(VERSION 3.22)


project(FRAM)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BINARY_DIR ./build)
set(CMAKE_BUILD_TYPE Release)
set(SeetaFaceDIR "/Users/aichao/Documents/GitHub/index/build")
set(CMAKE_PREFIX_PATH "/Users/aichao/Qt6/6.3.0/macos/lib/cmake")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


set(CMAKE_THREAD_LIBS_INIT "-lpthread")
#set(CMAKE_HAVE_THREADS_LIBRARY 1)
#set(CMAKE_USE_WIN32_THREADS_INIT 0)
#set(CMAKE_USE_PTHREADS_INIT 1)
#set(THREADS_PREFER_PTHREAD_FLAG ON)


find_package(jsoncpp CONFIG REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${SeetaFaceDIR}/include)
link_directories(${SeetaFaceDIR}/lib64)
find_package(OpenCV REQUIRED)
find_package(Qt6 COMPONENTS Core Gui Widgets Sql REQUIRED)
# opencv
list(APPEND LIBS ${OpenCV_LIBS})
#seeta
list(APPEND LIBS
        SeetaFaceLandmarker600
        SeetaFaceRecognizer610
        SeetaFaceDetector600
        SeetaFaceTracking600
        SeetaFaceAntiSpoofingX600
        )
#qt
list(APPEND LIBS Qt::Core Qt::Gui Qt::Widgets Qt::Sql)

file(GLOB HEAD ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB UI ${CMAKE_CURRENT_SOURCE_DIR}/ui/*)

message(STATUS ${UI})

add_executable(${PROJECT_NAME} ${SRC} ${HEAD} ${UI} main.cpp)
target_link_libraries(${PROJECT_NAME} ${LIBS})

