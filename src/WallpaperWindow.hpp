#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <SDL.h>
#include <string>

class WallpaperWindow {
public:
    explicit WallpaperWindow(const char* otherWindowAddr);
    bool Update();
    std::string GetAddress();
    ~WallpaperWindow();
private:
    static HWND GetWallpaperWindowHandle() ;
    static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

    HWND windowHandle;
    SDL_Window* window;
    SDL_GLContext context;

    int x, y;
};