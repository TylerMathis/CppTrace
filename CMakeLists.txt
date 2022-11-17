cmake_minimum_required(VERSION 3.16)
project(CppTrace)

set(CMAKE_CXX_STANDARD 17)

include(lib/CMakeLists.txt)
add_library(CppTraceLib ${CppTrace_SOURCES})

add_subdirectory(lib/extern/bvh)
target_link_libraries(CppTraceLib PUBLIC bvh)

add_executable(Render main.cpp)
target_link_libraries(Render CppTraceLib)

add_executable(Bench bench.cpp)
target_link_libraries(Bench CppTraceLib)
