cmake_minimum_required(VERSION ${CMAKE_VERSION})
project(DMTK VERSION 0.1)

add_definitions(-DGLEW_STATIC)

set(CMAKE_CXX_STANDARD 17)

##############################################################################
# Debug/Release mode:
#############################################################################
set(CMAKE_BUILD_TYPE "Debug")
if (CMAKE_BUILD_TYPE STREQUAL "Debug") 
add_compile_definitions(MIMIC_DEBUG = 1)
else (CMAKE_BUILD_TYPE STREQUAL "Release")
endif()

include_directories(
    src/mimic
    SDKs/sdl/include
    SDKs/sdl/include/SDL
    SDKs/glew/include
    SDKs/glm
    SDKs/assimp/include
    SDKs/imgui/include
    SDKs/stbi/include
    ${CMAKE_CURRENT_LIST_DIR}/SDKs/imgui/include
)

# Molebox: For combining .dll files - free & open-source:
link_directories(
    ${CMAKE_SOURCE_DIR}/SDKs/x64/lib
)

##############################################################################
# Mimic engine libraries: All libraries used within the engine.
#############################################################################
 add_library(MIMIC

    src/mimic/mimic_engine/Mimic.h
    src/mimic/mimic_engine/MimicCore.cpp
    src/mimic/mimic_engine/MimicCore.h
    src/mimic/mimic_engine/ResourceManager.cpp
    src/mimic/mimic_engine/ResourceManager.h
    src/mimic/mimic_engine/Resource.h
    src/mimic/mimic_engine/Environment.cpp
    src/mimic/mimic_engine/Environment.h
    src/mimic/mimic_engine/Texture.cpp
    src/mimic/mimic_engine/Texture.h
    src/mimic/mimic_engine/GameObject.cpp
    src/mimic/mimic_engine/GameObject.h
    src/mimic/mimic_engine/Component.cpp
    src/mimic/mimic_engine/Component.h
    src/mimic/mimic_engine/Model.cpp
    src/mimic/mimic_engine/Model.h
    src/mimic/mimic_engine/Shader.cpp
    src/mimic/mimic_engine/Shader.h
    src/mimic/mimic_engine/ModelRenderer.cpp
    src/mimic/mimic_engine/ModelRenderer.h
    src/mimic/mimic_engine/Material.cpp
    src/mimic/mimic_engine/Material.h
    src/mimic/mimic_engine/Camera.cpp
    src/mimic/mimic_engine/Camera.h
    src/mimic/mimic_engine/Light.cpp
    src/mimic/mimic_engine/Light.h
)

# Implement Mimic utility library:
add_library(MIMIC_UTILITY

    src/mimic/mimic_utility/Logger.cpp
    src/mimic/mimic_utility/Logger.h
    src/mimic/mimic_utility/PerformanceCounter.cpp
    src/mimic/mimic_utility/PerformanceCounter.h
    src/mimic/mimic_utility/FileLoader.cpp
    src/mimic/mimic_utility/FileLoader.h
)

# Implement Mimic render engine library:
add_library(MIMIC_RENDERER

    src/mimic/mimic_render/Model.cpp
    src/mimic/mimic_render/Model.h
    src/mimic/mimic_render/Mesh.cpp
    src/mimic/mimic_render/Mesh.h
    src/mimic/mimic_render/Shader.cpp
    src/mimic/mimic_render/Shader.h
    src/mimic/mimic_render/Vertex.cpp
    src/mimic/mimic_render/Vertex.h
    src/mimic/mimic_render/Texture.cpp
    src/mimic/mimic_render/Texture.h
    src/mimic/mimic_render/Renderer.cpp
    src/mimic/mimic_render/Renderer.h
    src/mimic/mimic_render/Light.cpp
    src/mimic/mimic_render/Light.h
    src/mimic/mimic_render/RenderTexture.cpp
    src/mimic/mimic_render/RenderTexture.h
    src/mimic/mimic_render/Window.cpp
    src/mimic/mimic_render/Window.h
    src/mimic/mimic_render/Camera.cpp
    src/mimic/mimic_render/Camera.h
    src/mimic/mimic_render/CubeMap.cpp
    src/mimic/mimic_render/CubeMap.h
    src/mimic/mimic_render/ShadowMapper.cpp
    src/mimic/mimic_render/ShadowMapper.h

    SDKs/stbi/include/stb_image.h
)

# Implement static IMGUI library:
add_library(IMGUI_LIB

    ${CMAKE_CURRENT_LIST_DIR}/SDKs/imgui/include/imgui.h
    ${CMAKE_CURRENT_LIST_DIR}/SDKs/imgui/include/imgui.cpp
    ${CMAKE_CURRENT_LIST_DIR}/SDKs/imgui/include/imconfig.h
    ${CMAKE_CURRENT_LIST_DIR}/SDKs/imgui/include/imgui_demo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/SDKs/imgui/include/imgui_draw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/SDKs/imgui/include/imgui_internal.h
    ${CMAKE_CURRENT_LIST_DIR}/SDKs/imgui/include/imstb_rectpack.h
    ${CMAKE_CURRENT_LIST_DIR}/SDKs/imgui/include/imstb_textedit.h
    ${CMAKE_CURRENT_LIST_DIR}/SDKs/imgui/include/imstb_truetype.h
    ${CMAKE_CURRENT_LIST_DIR}/SDKs/imgui/include/imgui_tables.cpp
    ${CMAKE_CURRENT_LIST_DIR}/SDKs/imgui/include/imgui_widgets.cpp

    ${CMAKE_CURRENT_LIST_DIR}/SDKs/imgui/include/imgui_impl_sdl2.cpp
    ${CMAKE_CURRENT_LIST_DIR}/SDKs/imgui/include/imgui_impl_sdl2.h
    ${CMAKE_CURRENT_LIST_DIR}/SDKs/imgui/include/imgui_impl_sdlrenderer2.cpp
    ${CMAKE_CURRENT_LIST_DIR}/SDKs/imgui/include/imgui_impl_sdlrenderer2.h
    ${CMAKE_CURRENT_LIST_DIR}/SDKs/imgui/include/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/SDKs/imgui/include/imgui_impl_opengl3.h
)

find_package(OpenGL)

#############################################################################
# Link libraries together, combining them into one library (MIMIC):
#############################################################################
target_link_libraries(MIMIC_RENDERER
    SDL2    
    OpenGL::GL
    assimp-vc143-mt
    glew32s
)

target_link_libraries(MIMIC
    IMGUI_LIB
    MIMIC_UTILITY
    MIMIC_RENDERER
)

#############################################################################
# DMTK program
#############################################################################
add_executable(DMTK
    src/dmtk/main.cpp
)

target_link_libraries(DMTK 
    MIMIC
)
