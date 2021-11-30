#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <shellapi.h>
#include <commctrl.h>
#include <SDL.h>
#include <random>
#include "resource.h"
#include "SDL_syswm.h"

#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    HWND p = FindWindowEx(hwnd, NULL, "SHELLDLL_DefView", NULL);
    HWND *ret = (HWND *) lParam;

    if (p) {
        *ret = FindWindowEx(NULL, hwnd, "WorkerW", NULL);
    }
    return true;
}

HWND get_wallpaper_window() {
    // Fetch the Progman window
    HWND progman = FindWindow("ProgMan", NULL);
    SendMessageTimeout(progman, 0x052C, 0, 0, SMTO_NORMAL, 1000, nullptr);
    HWND wallpaper_hwnd = nullptr;
    EnumWindows(EnumWindowsProc, (LPARAM) &wallpaper_hwnd);
    return wallpaper_hwnd;
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int ShowCmd) {

    // Get wallpaper window

    HWND window = get_wallpaper_window();

    // Get and display on the background

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *sdl_window = SDL_CreateWindowFrom((void *) window);
    SDL_Window *hidden_window = SDL_CreateWindow("Hidden Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 200, 200, SDL_WINDOW_HIDDEN);
    SDL_Renderer *renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    bool running = true;

    // Add item to taskbar status area

    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);

    NOTIFYICONDATA icon;
    if (SDL_GetWindowWMInfo(hidden_window, &info))
    {
        icon.uCallbackMessage = WM_USER + 1;
        icon.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
        icon.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
        icon.cbSize = sizeof(icon);
        icon.hWnd = info.info.win.window;
        icon.uVersion = NOTIFYICON_VERSION_4;
        strcpy_s(icon.szTip, "WallShader");

        Shell_NotifyIcon(NIM_DELETE, &icon);
        Shell_NotifyIcon(NIM_ADD, &icon);
        Shell_NotifyIcon(NIM_SETVERSION, &icon);
    }

    SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Welcome to WallShader!", "Thanks for choosing WallShader.\nTo close WallShader just click on it's icon in the tray.", hidden_window);

    // Random stuff

    std::mt19937_64 twister;

    int x, y;

    SDL_GetWindowSize(sdl_window, &x, &y);

    while (running) {
        // SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, twister() % 256, twister() % 256, twister() % 256, 255);
        SDL_RenderDrawLine(renderer, twister() % x, twister() % y, twister() % x, twister() % y);

        SDL_RenderPresent(renderer);

        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            switch (e.type)
            {
                case SDL_SYSWMEVENT:
                    if (e.syswm.msg->msg.win.msg == WM_USER + 1)
                    {
                        if (LOWORD(e.syswm.msg->msg.win.lParam) == WM_LBUTTONDOWN)
                        {
                            running = false;
                        }
                    }
                    break;
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }
    }
    Shell_NotifyIcon(NIM_DELETE, &icon);
    SDL_DestroyWindow(sdl_window);
    SDL_DestroyWindow(hidden_window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    DestroyWindow(window);
    
    return 0;
}
