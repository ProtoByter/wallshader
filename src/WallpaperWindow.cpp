#include "WallpaperWindow.hpp"
#include "SDL_opengl.h"

WallpaperWindow::WallpaperWindow(const char* otherWindowAddr) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);
    atexit(SDL_Quit);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_SetHint(SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT, otherWindowAddr);

    this->windowHandle = GetWallpaperWindowHandle();
    this->window = SDL_CreateWindowFrom(this->windowHandle);
    this->context = SDL_GL_CreateContext(this->window);

    SDL_GL_MakeCurrent(this->window, this->context);
    if (SDL_GL_SetSwapInterval(-1) == -1) {
        SDL_GL_SetSwapInterval(1);
    }

    SDL_GetWindowSize(this->window, &this->x, &this->y);
}

bool WallpaperWindow::Update() {
    SDL_GL_MakeCurrent(this->window, this->context);

    glViewport(0, 0, this->x, this->y);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(this->window);

    return false;
}

std::string WallpaperWindow::GetAddress() {
    std::string addr;
    addr.resize(32, '\0');
    sprintf_s(&addr[0], addr.size(), "%p", this->window);
    return addr;
}

WallpaperWindow::~WallpaperWindow() {
    SDL_DestroyWindow(this->window);
    SDL_GL_DeleteContext(this->context);
}

HWND WallpaperWindow::GetWallpaperWindowHandle() {
    // Fetch the ProgMan window
    HWND progman = FindWindow("ProgMan", NULL);
    SendMessageTimeout(progman, 0x052C, 0, 0, SMTO_NORMAL, 1000, nullptr);

    HWND wallpaper_hwnd = nullptr;
    EnumWindows(EnumWindowsProc, (LPARAM) &wallpaper_hwnd);
    return wallpaper_hwnd;
}

BOOL WallpaperWindow::EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    HWND shellDllHandle = FindWindowEx(hwnd, NULL, "SHELLDLL_DefView", NULL);
    HWND *return_ptr = (HWND *) lParam;

    if (shellDllHandle) {
        *return_ptr = FindWindowEx(NULL, hwnd, "WorkerW", NULL);
    }
    return true;
}
