cmake_minimum_required(VERSION 2.8)
project(examples)

add_executable(get_kinect_img ${CMAKE_CURRENT_SOURCE_DIR}/get_kinect_img.cpp)
add_executable(mapping_at_origin ${CMAKE_CURRENT_SOURCE_DIR}/mapping_at_origin.cpp)

target_link_libraries(get_kinect_img kinect ${libfreenect_LIBRARIES} ${OpenCV_LIBS})
target_link_libraries(mapping_at_origin kinect mapping ${libfreenect_LIBRARIES} ${OpenCV_LIBS})
