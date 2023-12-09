
cmake_minimum_required(VERSION 3.22)

file(GLOB SOURCES "*.cpp")

add_library(Renderer)

if (EMSCRIPTEN)
   target_link_libraries(Renderer PUBLIC glfw3 GL)
else()
   target_link_libraries(Renderer PUBLIC glfw GL)
endif(EMSCRIPTEN)


target_include_directories(Renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(Renderer PUBLIC ${SOURCES})

