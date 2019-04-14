cmake_minimum_required(VERSION 3.9)
project(Ray_Tracing)

set(CMAKE_CXX_STANDARD 11)
set(OpenCV_DIR /usr/local/Cellar/opencv/4.1.0)

find_package(OpenCV REQUIRED)
find_package (Eigen3 REQUIRED NO_MODULE)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${EIGEN3_INCLUDE_DIR})

add_executable(Ray_Tracing main.cpp surface.h surface.cpp ray_tracer.cpp ray_tracer.h)

set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})