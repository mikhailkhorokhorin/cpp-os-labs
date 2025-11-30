cmake_minimum_required(VERSION 3.10)
project(lab4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_library(basic SHARED lib/basic.cpp)
add_library(advanced SHARED lib/advanced.cpp)

set_target_properties(basic PROPERTIES OUTPUT_NAME "basic")
set_target_properties(advanced PROPERTIES OUTPUT_NAME "advanced")

add_executable(static app/static.cpp)
add_executable(dynamic app/dynamic.cpp)

target_link_libraries(static basic)
target_link_libraries(dynamic dl)