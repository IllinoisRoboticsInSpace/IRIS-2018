cmake_minimum_required(VERSION 2.8)
project(IRIS-2018)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake) #uncomment this if libfreenect needs to be manually found
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

option(DEBUG "set to ON to show debug information and image plot" OFF)

add_compile_options(-std=c++14)

find_package(OpenCV REQUIRED)
find_package(libfreenect REQUIRED)
#find_package(Threads REQUIRED)

include_directories(${OpenCV_INLUCDE_DIRS})
include_directories("/usr/include/libusb-1.0/") #TODO: use module to handle path finding
include_directories(${CMAKE_SOURCE_DIR}/kinect)
include_directories(${CMAKE_SOURCE_DIR}/examples)
include_directories(${CMAKE_SOURCE_DIR}/mapping)
include_directories(${CMAKE_SOURCE_DIR}/CSK)
#include_directories(${CMAKE_SOURCE_DIR}/path_planning) #not finished yet


if (DEBUG)
    add_definitions(-DDEBUG)
endif()

add_subdirectory(CSK)
add_subdirectory(kinect)
add_subdirectory(mapping)
add_subdirectory(examples)
