cmake_minimum_required(VERSION 3.15)
project(dijkstra_spfa)

OPTION(BUILD_DEBUG "Debug build" ON)

if (BUILD_DEBUG)
    TARGET_COMPILE_DEFINITIONS(dijkstra_spfa PUBLIC BUILD_DEBUG)
endif ()

set(CMAKE_CXX_STANDARD 17)

add_executable(dijkstra_spfa main.cpp)