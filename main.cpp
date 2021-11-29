#include <windows.h>
#include <SDL2/SDL.h>

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    HWND p = FindWindowExW(hwnd, NULL, L"SHELLDLL_DefView", NULL);
    HWND* ret = (HWND*)lParam;

    if (p)
    {
        // Gets the WorkerW Window after the current one.
        *ret = FindWindowExW(NULL, hwnd, L"WorkerW", NULL);
    }
    return true;
}

HWND get_wallpaper_window()
{
    // Fetch the Progman window
    HWND progman = FindWindowW(L"ProgMan", NULL);
    // Send 0x052C to Progman. This message directs Progman to spawn a 
    // WorkerW behind the desktop icons. If it is already there, nothing 
    // happens.
    SendMessageTimeout(progman, 0x052C, 0, 0, SMTO_NORMAL, 1000, nullptr);
    // We enumerate all Windows, until we find one, that has the SHELLDLL_DefView 
    // as a child. 
    // If we found that window, we take its next sibling and assign it to workerw.
    HWND wallpaper_hwnd = nullptr;
    EnumWindows(EnumWindowsProc, (LPARAM)&wallpaper_hwnd);
    // Return the handle you're looking for.
    return wallpaper_hwnd;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int ShowCmd) {
    SDL_Init(SDL_INIT_VIDEO);
    HWND window = get_wallpaper_window();
    SDL_Window* sdl_window = SDL_CreateWindowFrom((void*)window);
    SDL_Renderer* renderer = SDL_CreateRenderer(sdl_window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}
