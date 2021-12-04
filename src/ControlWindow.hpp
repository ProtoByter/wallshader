#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <SDL.h>
#include <shellapi.h>
#include <string>

#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>

class ControlWindow {
public:
    explicit ControlWindow(HINSTANCE hInstance);
    bool Update();
    std::string GetAddress();
    ~ControlWindow();
private:
    SDL_Window *window;
    SDL_GLContext context;
    NOTIFYICONDATA iconData;

    bool control_window_shown = false;
    bool editor_window_shown = false;
    bool stats_window_shown = false;

    ImGuiIO* io;
};
