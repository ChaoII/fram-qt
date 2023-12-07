cmake_minimum_required(VERSION 3.5)

project(CustomSpinBox)

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


#add include directory
include_directories(${CMAKE_SOURCE_DIR})

# add src
aux_source_directory(./ SWITCH_SRC)


add_library(CustomSpinBox ${SWITCH_SRC} spinbox.ui spinbox.qrc)
target_link_libraries(CustomSpinBox ${DEPENDS})
