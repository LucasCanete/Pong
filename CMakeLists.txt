cmake_minimum_required(VERSION 4.3)
project(pong C)

set(CMAKE_C_STANDARD 11)
find_package(raylib REQUIRED)

add_executable(pong main.c)
target_link_libraries(pong PRIVATE raylib)