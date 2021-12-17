cmake_minimum_required(VERSION 3.0)
project(wallshader)
enable_language(RC)

if (NOT WIN32)
    message(FATAL_ERROR "This only works on Windows")
endif()

include(FindOpenGL)
#find_package(glbinding REQUIRED)

# Dependencies

include(depends/sdl2.cmake)
getSDL2(2.0.16)

add_subdirectory(nall)
add_subdirectory(hiro)

add_library(WallShaderShared SHARED
        src/SharedLogic/Project.cpp
        src/SharedLogic/Utils.cpp
)

target_include_directories(WallShaderShared PRIVATE src/SharedLogic ${sdl2_SOURCE_DIR}/include)

target_link_libraries(WallShaderShared PUBLIC SDL2 OpenGL::GL glbinding glbinding-aux)

add_executable(WallShaderControl WIN32
        src/ControlPanel/main.cpp
)

target_link_libraries(WallShaderControl PRIVATE nall hiro WallShaderShared)
target_include_directories(WallShaderControl PRIVATE
        ${sdl2_SOURCE_DIR}/include
        resources/
        src/SharedLogic/
)

add_executable(WallShaderBackground WIN32
        src/Wallpaper/main.cpp
)

target_link_libraries(WallShaderBackground PRIVATE SDL2 Comctl32 OpenGL::GL glbinding glbinding-aux nall WallShaderShared)
target_include_directories(WallShaderBackground PRIVATE
        ${sdl2_SOURCE_DIR}/include
        resources/
        src/SharedLogic/
)

add_custom_target(WallShader)
add_dependencies(WallShader WallShaderBackground WallShaderControl)