
cmake_minimum_required(VERSION 3.22)

file(GLOB SOURCES "*.cpp")

add_library(Utils)


target_include_directories(Utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(Utils PUBLIC ${SOURCES})

