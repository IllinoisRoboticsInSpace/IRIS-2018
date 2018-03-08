cmake_minimum_required (VERSION 2.8)
project(IRIS-2018)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(OpenCV REQUIRED)
find_package(libfreenect REQUIRED)

include_directories("/usr/include/libusb-1.0/")

add_executable(mapping_test test_main.cpp
                            test2/kinectDevice.cpp
                            mapping/mapping.cpp
                    )

add_executable(kinect_test test2/main.cpp
                           test2/kinectDevice.cpp
                    )
