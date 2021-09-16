cmake_minimum_required(VERSION 3.21)

# set the project name
project(ComputerGraphics)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories("third parties/glm" 
                    "third parties/stb" 
                    ${CMAKE_SOURCE_DIR} 
                    "third parties/fmt/include")

find_package(GLEW NAMES glew glew32 GLEW PATHS "${CMAKE_CURRENT_SOURCE_DIR}/third parties/glew"  CONFIG REQUIRED)
find_package(GLFW NAMES glfw3 PATHS "${CMAKE_CURRENT_SOURCE_DIR}/third parties/glfw" CONFIG REQUIRED)
# add the executable
add_executable(cg
    camera.cpp
    engine.cpp
    Item.cpp
    items_setting.cpp
    obj_reader.cpp
    shader.cpp
    shadowManager.cpp
    Source.cpp
    world_generation/grass.cpp
    world_generation/terrain.cpp
    world_generation/terrainGenerator.cpp
    world_generation/terrainHandler.cpp
    world_generation/trees.cpp
    world_generation/world_chunk.cpp
    texture.cpp
    UBO.cpp
    WaterSimulation.cpp)

target_link_libraries(cg PUBLIC GLEW::GLEW glfw "${CMAKE_CURRENT_SOURCE_DIR}/third parties/fmt/libfmt.a")

add_custom_target(CopyBinaries
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/third parties/glew/bin/glew32.dll" ${CMAKE_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/obj" ${CMAKE_BINARY_DIR}/obj
)

add_dependencies(cg CopyBinaries)