cmake_minimum_required(VERSION 3.20)
project(pong C)

set(CMAKE_C_STANDARD 11)
find_package(raylib REQUIRED)


# Uses raylib installed if exists; if not it downloads it
find_package(raylib 5.5 QUIET)
if(NOT raylib_FOUND)
    FetchContent_Declare(
            raylib
            GIT_REPOSITORY https://github.com/raysan5/raylib.git
            GIT_TAG 5.5
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(raylib)
endif()

add_executable(pong main.c)
target_link_libraries(pong PRIVATE raylib)