cmake_minimum_required(VERSION 3.22)
project(CppTrace)

set(CMAKE_CXX_STANDARD 17)

include(lib/CMakeLists.txt)

add_executable(CppTrace bench.cpp ${CppTrace_SOURCES})
