cmake_minimum_required(VERSION 3.5)
project(simple)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILE main.cpp)
add_executable(simple ${SOURCE_FILES})

set(PYTHON_INCLUDE_DIR D:/Python27/include)
set(PYTHON_LIBRARY D:/Python27/libs)

include_directories(${PYTHON_INCLUDE_DIR})
link_directories(${PYTHON_LIBRARY})