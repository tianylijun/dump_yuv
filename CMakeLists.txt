project(opencv-demo)

cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "/home/leejohnnie/3rd/opencv-2.4.13/build-pc")

find_package(OpenCV REQUIRED)

add_executable(dump_yuv  dump_yuv.cpp)     

target_link_libraries(dump_yuv ${OpenCV_LIBS}) 
