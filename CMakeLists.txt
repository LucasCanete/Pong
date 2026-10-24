cmake_minimum_required(VERSION 3.20)
project(pong C)

set(CMAKE_C_STANDARD 11)

include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

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

add_executable(pong
        main.c
        src/racket.c
        src/ball.c
        src/field.c
        src/game.c
)

target_include_directories(pong PRIVATE include)
target_link_libraries(pong PRIVATE raylib)