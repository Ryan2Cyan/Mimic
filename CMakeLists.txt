cmake_minimum_required(VERSION ${CMAKE_VERSION})
project(DMTK VERSION 0.1)

add_definitions(-DGLEW_STATIC)

set(CMAKE_CXX_STANDARD 17)

# Set Debug/Release mode:
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
)

# Molebox: For combining .dll files - free & open-source:
link_directories(
    ${CMAKE_SOURCE_DIR}/SDKs/x64/lib
)

################################################################i##############
# DMTK engine
#############################################################################
add_library(mimic
    src/mimic/lowlevelsystems/Mimic.h
    src/mimic/lowlevelsystems/MimicCore.cpp
    src/mimic/lowlevelsystems/MimicCore.h
    src/mimic/lowlevelsystems/GameObject.cpp
    src/mimic/lowlevelsystems/GameObject.h
    src/mimic/lowlevelsystems/Component.cpp
    src/mimic/lowlevelsystems/Component.h
    src/mimic/lowlevelsystems/Window.cpp
    src/mimic/lowlevelsystems/Window.h
    src/mimic/lowlevelsystems/Environment.cpp
    src/mimic/lowlevelsystems/Environment.h
    src/mimic/lowlevelsystems/Camera.cpp
    src/mimic/lowlevelsystems/Camera.h
    src/mimic/lowlevelsystems/ResourceManager.cpp
    src/mimic/lowlevelsystems/ResourceManager.h
    src/mimic/lowlevelsystems/ModelRenderer.cpp
    src/mimic/lowlevelsystems/ModelRenderer.h
    src/mimic/lowlevelsystems/Resource.cpp
    src/mimic/lowlevelsystems/Resource.h

    src/mimic/utility/Logger.cpp
    src/mimic/utility/Logger.h
    src/mimic/utility/PerformanceCounter.cpp
    src/mimic/utility/PerformanceCounter.h
    src/mimic/utility/FileLoader.cpp
    src/mimic/utility/FileLoader.h
)

# Make the renderer a separate library that's attached to the mimic engine.
add_library(mimicrenderer
    src/mimic/renderengine/Model.cpp
    src/mimic/renderengine/Model.h
    src/mimic/renderengine/Mesh.cpp
    src/mimic/renderengine/Mesh.h
    src/mimic/renderengine/Shader.cpp
    src/mimic/renderengine/Shader.h
    src/mimic/renderengine/Vertex.cpp
    src/mimic/renderengine/Vertex.h
    src/mimic/renderengine/Texture.cpp
    src/mimic/renderengine/Texture.h
    src/mimic/renderengine/Renderer.cpp
    src/mimic/renderengine/Renderer.h
    src/mimic/renderengine/Material.cpp
    src/mimic/renderengine/Material.h

    SDKs/stbi/include/stb_image.h
)

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

set(SPDLOG_PATH ${CMAKE_CURRENT_LIST_DIR}/SDKs/imgui/include)
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

find_package(OpenGL)

target_link_libraries(mimic
    SDL2    
    OpenGL::GL
    assimp-vc143-mt
    glew32s
    ${IMGUI_LIB}
    mimicrenderer
)

#target_link_libraries(mimicrenderer
#    SDL2    
#    OpenGL::GL
#    assimp-vc143-mt
#    glew32s
#)



#############################################################################
# DMTK program
#############################################################################
add_executable(dmtk
    src/dmtk/main.cpp
)

target_link_libraries(dmtk 
    mimic
)
