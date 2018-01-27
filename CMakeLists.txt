cmake_minimum_required(VERSION 3.9)
project(Ray_Tracing)

set(CMAKE_CXX_STANDARD 11)
set(OpenCV_DIR "C:/OpenCV/build")

find_package(OpenCV REQUIRED)

include_directories("C:/Program Files/Eigen/eigen-eigen-5a0156e40feb")
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Ray_Tracing main.cpp surface.h surface.cpp ray_tracer.cpp ray_tracer.h)

set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})