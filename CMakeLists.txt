cmake_minimum_required(VERSION 3.14)
include_directories(.)

add_subdirectory(delaunayTriangulation)
find_package(OpenCV REQUIRED)
add_executable(main main.cpp)
target_link_libraries(main PRIVATE opencv_core opencv_highgui delaunay_lib)

