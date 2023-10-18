cmake_minimum_required(VERSION ${CMAKE_VERSION})
project(DMTK VERSION 0.1)


include_directories(
    src/mimic
    SDKs/sdl2/include
    SDKs/glew/include
    SDKs/glm
    SDKs/assimp/include
)

link_directories(
    SDKs/sdl2/lib64
    SDKs/glew/lib/Release/x64
    SDKs/assimp/lib/x64
)

################################################################i##############
# DMTK engine
#############################################################################
add_library(mimic
    src/mimic/lowlevelsystems/Core.cpp
    src/mimic/lowlevelsystems/Core.h
    src/mimic/lowlevelsystems/GameObject.cpp
    src/mimic/lowlevelsystems/GameObject.h
    src/mimic/lowlevelsystems/Component.cpp
    src/mimic/lowlevelsystems/Component.h

    src/mimic/renderengine/Renderer.cpp
    src/mimic/renderengine/Renderer.h
    src/mimic/renderengine/Mesh.cpp
    src/mimic/renderengine/Mesh.h
    src/mimic/renderengine/Shader.cpp
    src/mimic/renderengine/Shader.h
  #  src/mimic/VertexShader.txt
  #  src/mimic/FragmentShader.txt
)

target_link_libraries(mimic
    SDL2
    GLEW32
    OPENGL32
    assimp-vc143-mt
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
