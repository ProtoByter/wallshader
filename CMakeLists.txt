cmake_minimum_required(VERSION 3.0)
project(wallshader)
enable_language(RC)

if (NOT WIN32)
    message(FATAL_ERROR "This only works on Windows")
endif()

include(FindOpenGL)

include(depends/sdl2.cmake)

add_custom_target(
        main.o
        COMMAND ${CMAKE_RC_COMPILER} main.rc main.o
        DEPENDS icon.ico
        SOURCES main.rc
)

add_executable(WallShader WIN32
        main.cpp main.rc
        imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp
        imgui/backends/imgui_impl_sdl.cpp imgui/backends/imgui_impl_opengl3.cpp
)

target_link_libraries(WallShader SDL2-static Comctl32 OpenGL::GL)
target_include_directories(WallShader PRIVATE
        ${sdl2_SOURCE_DIR}/include
        imgui/ imgui/backends/
)