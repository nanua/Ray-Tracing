cmake_minimum_required(VERSION 3.9)
project(Ray_Tracing)

set(CMAKE_CXX_STANDARD 11)

set(OpenCV_DIR "C:/Program Files/OpenCV/my_build")
# set(BOOST_ROOT "C:/boost/boost_1_66_0")


find_package(OpenCV REQUIRED)
# find_package(Boost REQUIRED)

include_directories("C:/Program Files/Eigen/eigen-eigen-5a0156e40feb")
# include_directories(${Boost_INCLUDE_DIRS})
add_executable(Ray_Tracing main.cpp surface.h surface.cpp ray_tracer.cpp ray_tracer.h)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})