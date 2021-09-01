cmake_minimum_required(VERSION 3.21)

# set the project name
project(ComputerGraphics)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories("third parties/glm" "third parties/stb")

find_package(GLEW NAMES glew glew32 GLEW PATHS "${CMAKE_CURRENT_SOURCE_DIR}/third parties/glew"  CONFIG REQUIRED)
find_package(GLFW NAMES glfw3 PATHS "${CMAKE_CURRENT_SOURCE_DIR}/third parties/glfw" CONFIG REQUIRED)

# add the executable
add_executable(cg
    camera.cpp
    GaussianBlur.cpp
    engine.cpp
    Item.cpp
    items_setting.cpp
    lightSource.cpp
    obj_reader.cpp
    shader.cpp
    shadowManager.cpp
    Source.cpp
    texture.cpp
    UBO.cpp
    Utils.cpp
    WaterSimulation.cpp)

target_link_libraries(cg GLEW::GLEW glfw)