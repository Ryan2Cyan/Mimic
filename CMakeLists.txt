cmake_minimum_required(VERSION ${CMAKE_VERSION})
project(DMTK VERSION 0.1)

add_definitions(-DGLEW_STATIC)

include_directories(
    src/mimic
    SDKs/sdl/include
    SDKs/sdl/include/SDL
    SDKs/glew/include
    SDKs/glm
    SDKs/assimp/include
    SDKs/imgui/include
)

link_directories(
    ${CMAKE_SOURCE_DIR}/SDKs/x64/lib
)

################################################################i##############
# DMTK engine
#############################################################################
add_library(mimic
    src/mimic/lowlevelsystems/Mimic.h
    src/mimic/lowlevelsystems/Core.cpp
    src/mimic/lowlevelsystems/Core.h
    src/mimic/lowlevelsystems/GameObject.cpp
    src/mimic/lowlevelsystems/GameObject.h
    src/mimic/lowlevelsystems/Component.cpp
    src/mimic/lowlevelsystems/Component.h
    src/mimic/lowlevelsystems/Window.cpp
    src/mimic/lowlevelsystems/Window.h

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
find_library (GLEW_LIB      NAMES glew32s              PATHS ${CMAKE_SOURCE_DIR}/SDKs/x64/lib         REQUIRED)

# Implement static IMGUI:
set(IMGUI_PATH ${CMAKE_CURRENT_LIST_DIR}/SDKs/imgui/include)
file(GLOB IMGUI_GLOB
    ${IMGUI_PATH}/imgui.h
    ${IMGUI_PATH}/imgui.cpp
    ${IMGUI_PATH}/imconfig.h
    ${IMGUI_PATH}/imgui_demo.cpp
    ${IMGUI_PATH}/imgui_draw.cpp
    ${IMGUI_PATH}/imgui_internal.h
    ${IMGUI_PATH}/imstb_rectpack.h
    ${IMGUI_PATH}/imstb_textedit.h
    ${IMGUI_PATH}/imstb_truetype.h
    ${IMGUI_PATH}/imgui_tables.cpp
    ${IMGUI_PATH}/imgui_widgets.cpp

    ${IMGUI_PATH}/imgui_impl_sdl2.cpp
    ${IMGUI_PATH}/imgui_impl_sdl2.h
    ${IMGUI_PATH}/imgui_impl_sdl2renderer.cpp
    ${IMGUI_PATH}/imgui_impl_sdl2renderer.h
    ${IMGUI_PATH}/imgui_impl_opengl3.cpp
    ${IMGUI_PATH}/imgui_impl_opengl3.h
    ${IMGUI_PATH}/imgui_impl_opengl3_loader.cpp
)
add_library("IMGUI_LIB" STATIC ${IMGUI_GLOB})
target_include_directories("IMGUI_LIB" PUBLIC ${IMGUI_PATH})
target_link_libraries("IMGUI_LIB" PRIVATE SDL2_LIB)

#target_compile_definitions(
#    -DSDL_MAIN_HANDLED
#)

# include OpenGL:
find_package(OpenGL)

# print file-paths to make sure cmake is using the correct paths:
message(STATUS	"SDL2 Path:" ${SDL2_LIB})
message(STATUS	"ASSIMP Path:" ${ASSIMP_LIB})
message(STATUS	"GLEW Path:" ${GLEW_LIB})
message(STATUS  "ImGui Path: ${IMGUI_PATH}")

target_link_libraries(mimic
    ${SDL2_LIB}
    OpenGL::GL
    ${ASSIMP_LIB}
    #${GLEW_LIB}
    glew32s
    ${IMGUI_LIB}
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
