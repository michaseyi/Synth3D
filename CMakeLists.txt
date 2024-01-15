cmake_minimum_required(VERSION 3.22)
project(Synth3d)

# target_include_directories(Synth3d PUBLIC external/OpenMesh)
include_directories(external/OpenMesh/src)

add_subdirectory(src)
add_subdirectory(src/Renderer)
add_subdirectory(src/Utils)
add_subdirectory(src/Core)
add_subdirectory(src/Math)
add_subdirectory(external/glad)

# add_subdirectory(external/OpenMesh)

if(EMSCRIPTEN)
    target_link_directories(Synth3d ${CMAKE_LINK_DIRECTORIES_BEFORE} PUBLIC lib)
else()
    find_package(OpenMesh REQUIRED)
endif()

target_link_libraries(Synth3d Core Renderer Utils Math glad OpenMeshTools OpenMeshCore)

set_target_properties(Synth3d  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

# set_target_properties(Renderer Core Utils Math glad OpenMeshTools OpenMeshCore PROPERTIES 
#     ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")