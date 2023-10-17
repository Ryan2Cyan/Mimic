cmake_minimum_required(VERSION ${CMAKE_VERSION})
project(DMTK VERSION 0.1)


include_directories(
    src
    SDKs/sdl2/include
    SDKs/glew/include
    SDKs/glm
)

link_directories(
    SDKs/sdl2/lib64
    SDKs/glew/lib/Release/x64
)

#############################################################################
# DMTK engine
#############################################################################
add_library(mimic
    src/mimic/Core.cpp
    src/mimic/Core.h
    src/mimic/Entity.cpp
    src/mimic/Entity.h
    src/mimic/Component.cpp
    src/mimic/Component.h
)

target_link_libraries(mimic
    SDL2
    GLEW32
    OPENGL32
)

#############################################################################
# DMTK program
#############################################################################
add_executable(dmtk
    src/dmtk/main.cpp
)

target_link_libraries(dmtk 
    mimic
)