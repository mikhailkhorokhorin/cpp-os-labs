cmake_minimum_required(VERSION 3.10)
project(lab2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(main main.cpp)

find_package(Threads REQUIRED)
target_link_libraries(main PRIVATE Threads::Threads)