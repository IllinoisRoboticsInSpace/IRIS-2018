cmake_minimum_required (VERSION 2.8)
project(kinect)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(SOURCES kinectDevice.cpp)

add_library(kinect ${SOURCES})

target_link_libraries(kinect ${OpenCV_LIBS} ${libfreenect_LIBRARIES} pthread)
