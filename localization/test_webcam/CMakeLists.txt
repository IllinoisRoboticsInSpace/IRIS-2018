cmake_minimum_required(VERSION 2.8)
project( checkboard_navigation )
find_package( OpenCV REQUIRED )
find_package (Threads)
add_executable( checkboard_navigation main.cpp ../src/checkboard_navigation_module.cpp )

include_directories( /usr/include/ )
include_directories( ${OpenCV_INCLUDE_DIRS})
target_link_libraries (checkboard_navigation ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} -lz -lserial)
