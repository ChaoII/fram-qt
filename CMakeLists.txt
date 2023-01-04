cmake_minimum_required(VERSION 3.5)

project(FRAM VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})


# 添加FastDeploy依赖头文件
include(/Users/aichao/Documents/GitHub/FastDeploy/build/install/FastDeploy.cmake)
include_directories(${FASTDEPLOY_INCS})

# 添加faiss
set(FAISS_DIR /Users/aichao/CLionProjects/third/faiss)
find_package(faiss REQUIRED PATHS ${FAISS_DIR}/share/faiss NO_DEFAULT_PATH)
include_directories("${FAISS_DIR}/include")
link_directories("${FAISS_DIR}/lib")

# 添加opencv，在fastdeploy中已经有opencv
#set(OpenCV_DIR "/Users/aichao/CLionProjects/third/opencv/release/lib64/cmake/opencv4" CACHE PATH "opencv path" FORCE)
#find_package(OpenCV REQUIRED NO_DEFAULT_PATH)

#==============source_dir==============
set(ALL_SOURCES "")
file(GLOB_RECURSE UI_SOURCE ${PROJECT_SOURCE_DIR}/ui/*.cpp)
file(GLOB_RECURSE COMMON_SOURCE ${PROJECT_SOURCE_DIR}/common/*)
file(GLOB_RECURSE CORE_SOURCE ${PROJECT_SOURCE_DIR}/core/*.cpp)
file(GLOB RESOURCE_FILE ${PROJECT_SOURCE_DIR}/ui/QResource/resource.qrc)

list(APPEND ALL_SOURCES ${UI_SOURCE} ${COMMON_SOURCE} ${RESOURCE_FILE} ${CORE_SOURCE} main.cpp)

add_executable(FRAM ${ALL_SOURCES})

target_link_libraries(FRAM PUBLIC Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql  ${FASTDEPLOY_LIBS} faiss.dylib)


install(TARGETS FRAM
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(FRAM)
endif()
