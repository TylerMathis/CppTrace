cmake_minimum_required(VERSION 3.16)
project(CppTrace)

set(CMAKE_CXX_STANDARD 17)

include(lib/CMakeLists.txt)

add_executable(CppTrace main.cpp ${CppTrace_SOURCES})
