cmake_minimum_required(VERSION 3.10)
project(lab3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(main main.cpp)
add_executable(child child.cpp)

target_link_libraries(main rt)
target_link_libraries(child rt)