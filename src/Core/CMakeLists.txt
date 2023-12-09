
cmake_minimum_required(VERSION 3.22)

file(GLOB SOURCES "*.cpp")

add_library(Core)

target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(Core PUBLIC ${SOURCES})

