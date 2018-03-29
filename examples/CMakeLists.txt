cmake_minimum_required(VERSION 2.8)
project(examples)

add_executable(get_kinect_img ${CMAKE_CURRENT_SOURCE_DIR}/get_kinect_img.cpp)

target_link_libraries(get_kinect_img kinect)
