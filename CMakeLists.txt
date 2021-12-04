cmake_minimum_required(VERSION 3.0)
project(wallshader)
enable_language(RC)

if (NOT WIN32)
    message(FATAL_ERROR "This only works on Windows")
endif()

include(FindOpenGL)

include(depends/sdl2.cmake)

add_executable(WallShader WIN32
        src/main.cpp resources/main.rc
        imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp
        imgui/backends/imgui_impl_sdl.cpp imgui/backends/imgui_impl_opengl3.cpp
        src/WallpaperWindow.cpp src/WallpaperWindow.hpp src/ControlWindow.cpp src/ControlWindow.hpp)

target_link_libraries(WallShader SDL2-static Comctl32 OpenGL::GL)
target_include_directories(WallShader PRIVATE
        ${sdl2_SOURCE_DIR}/include
        imgui/ imgui/backends/
        resources/
)
