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


set(EXTERN_DEPS "")
set(EXT_DIR "")
if(WIN32)
    set(EXT_DIR E:/develop)
elseif(APPLE)
    set(EXT_DIR /Users/aichao/CLionProjects/third)
else()
    message(FATAL "only supported by windows and macos")
endif()

# OpenMP

find_package(OpenMP REQUIRED)
if(OPENMP_FOUND)
    message("Found OpenMP")
    set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS})
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS})
    set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS})
endif()




# 添加FastDeploy依赖头文件
if (WIN32)
    include(${EXT_DIR}/fastdeploy/FastDeploy.cmake)
else()
    include(${EXT_DIR}/fastdeploy/FastDeploy.cmake)
endif()
include_directories(${FASTDEPLOY_INCS})
list(APPEND EXTERN_DEPS ${FASTDEPLOY_LIBS})


# 添加faiss
if(WIN32)
    set(FAISS_DIR E:/develop/minconda3/Library)
    include_directories("${FAISS_DIR}/include")
    link_directories("${FAISS_DIR}/lib")
    list(APPEND EXTERN_DEPS faiss)
else()
    set(FAISS_DIR ${EXT_DIR}/faiss)
    find_package(faiss REQUIRED PATHS ${FAISS_DIR}/share/faiss NO_DEFAULT_PATH)
    include_directories("${FAISS_DIR}/include")
    link_directories("${FAISS_DIR}/lib")
    list(APPEND EXTERN_DEPS faiss)
endif()
# 添加QxOrm库
set(QxOrm_DIR ${EXT_DIR}/QxOrm)
include_directories("${QxOrm_DIR}/include")
link_directories("${QxOrm_DIR}/lib")
list(APPEND EXTERN_DEPS QxOrm)

# 添加opencv，在fastdeploy中已经有opencv
#set(OpenCV_DIR "/Users/aichao/CLionProjects/third/opencv/release/lib64/cmake/opencv4" CACHE PATH "opencv path" FORCE)
#find_package(OpenCV REQUIRED NO_DEFAULT_PATH)
#==============source_dir==============
set(ALL_SOURCES "")
file(GLOB_RECURSE UI_SOURCE ${PROJECT_SOURCE_DIR}/ui/*.cpp ${PROJECT_SOURCE_DIR}/ui/*.h)
file(GLOB_RECURSE COMMON_SOURCE ${PROJECT_SOURCE_DIR}/common/*)
file(GLOB_RECURSE CORE_SOURCE ${PROJECT_SOURCE_DIR}/core/*)
file(GLOB_RECURSE PLUGIN_SOURCE ${PROJECT_SOURCE_DIR}/plugin/*)
file(GLOB RESOURCE_FILE ${PROJECT_SOURCE_DIR}/ui/QResource/resource.qrc)

list(APPEND ALL_SOURCES ${UI_SOURCE} ${COMMON_SOURCE} ${RESOURCE_FILE} ${CORE_SOURCE} ${PLUGIN_SOURCE} main.cpp)

add_executable(FRAM ${ALL_SOURCES})

target_link_libraries(FRAM PUBLIC Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql  ${EXTERN_DEPS})


install(TARGETS FRAM
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(FRAM)
endif()
