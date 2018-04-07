cmake_minimum_required(VERSION 2.8)
project(mapping)

#set(SOURCES mapping.cpp)
set(SOURCES kinect_pcl.cpp)

add_library(mapping ${SOURCES})

target_link_libraries(mapping CSK)
