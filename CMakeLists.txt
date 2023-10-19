cmake_minimum_required(VERSION ${CMAKE_VERSION})
project(DMTK VERSION 0.1)


include_directories(
    src/mimic
    SDKs/sdl/include
    SDKs/glew/include
    SDKs/glm
    SDKs/assimp/include
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
)

# tell cmake where all the .lib files are (linking from source):
find_library (SDL2_LIB      NAMES SDL2                 PATHS ${CMAKE_SOURCE_DIR}/SDKs/x64/lib         REQUIRED)
find_library (ASSIMP_LIB    NAMES assimp-vc143-mt      PATHS ${CMAKE_SOURCE_DIR}/SDKs/x64/lib         REQUIRED)
find_library (GLEW_LIB      NAMES libglew32            PATHS ${CMAKE_SOURCE_DIR}/SDKs/x64/lib         REQUIRED)

# ensure GLEW compiles statically:
add_definitions(-DGLEW_STATIC)
add_compile_definitions(
  GLEW_STATIC
)

#target_compile_definitions(
#    -DSDL_MAIN_HANDLED
#)

# include OpenGL:
find_package(OpenGL)

# print file-paths to make sure cmake is using the correct paths:
message(STATUS	"SDL2 Path:" ${SDL2_LIB})
message(STATUS	"ASSIMP Path:" ${ASSIMP_LIB})
message(STATUS	"GLEW Path:" ${GLEW_LIB})

target_link_libraries(mimic
    ${SDL2_LIB}
    OpenGL::GL
    ${ASSIMP_LIB}
    ${GLEW_LIB}
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
