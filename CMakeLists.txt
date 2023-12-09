cmake_minimum_required(VERSION 3.22)
project(Synth3d)

add_subdirectory(src)
add_subdirectory(src/Renderer)
add_subdirectory(src/Utils)
add_subdirectory(src/Core)


add_subdirectory(external/glad)
add_subdirectory(external/ecs)

find_package(OpenMesh REQUIRED)

target_include_directories(Synth3d PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(Synth3d Core Renderer Utils glad OpenMeshCore OpenMeshTools ecs)


set_target_properties(Synth3d  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

# set_target_properties(Renderer Core Utils PROPERTIES ecs glad
#     ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")