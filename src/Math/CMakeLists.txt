
cmake_minimum_required(VERSION 3.22)

file(GLOB SOURCES "*.cpp")

add_library(Math)

target_include_directories(Math PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(Math PUBLIC ${SOURCES})

