cmake_minimum_required(VERSION 3.14)
project(try)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES backgroud_delete.cpp count.h count.cpp)

add_executable(Video ${SOURCE_FILES})
target_link_libraries(Video ${OpenCV_LIBS}) # 链接库