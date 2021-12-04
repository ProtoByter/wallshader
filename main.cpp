#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <shellapi.h>
#include <SDL.h>
#include <random>
#include "resource.h"
#include "SDL_syswm.h"
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>
#include <SDL_opengl.h>

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
    // Fetch the ProgMan window
    HWND progman = FindWindow("ProgMan", NULL);
    SendMessageTimeout(progman, 0x052C, 0, 0, SMTO_NORMAL, 1000, nullptr);
    HWND wallpaper_hwnd = nullptr;
    EnumWindows(EnumWindowsProc, (LPARAM) &wallpaper_hwnd);
    return wallpaper_hwnd;
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int ShowCmd) {

    // Get wallpaper wallpaperWindow

    HWND wallpaperWindow = get_wallpaper_window();

    // Init SDL

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);
    SDL_Window* hidden_window = SDL_CreateWindow("Hidden Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 200, 200, SDL_WINDOW_HIDDEN);

    SDL_Window* control_window = SDL_CreateWindow("WallShader Control Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    char address[32];
    sprintf(address, "%p", control_window);

    SDL_SetHint(SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT, address);

    SDL_Window* wallpaperWindowSDL = SDL_CreateWindowFrom((void *)wallpaperWindow);

    bool running = true;

    // Set up ImGUI

    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GLContext control_gl_ctx = SDL_GL_CreateContext(control_window);
    SDL_GL_MakeCurrent(control_window, control_gl_ctx);
    SDL_GL_SetSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(control_window, control_gl_ctx);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Create OpenGL Context for Background Window

    SDL_GLContext wallpaper_gl_ctx = SDL_GL_CreateContext(wallpaperWindowSDL);
    SDL_GL_MakeCurrent(wallpaperWindowSDL, wallpaper_gl_ctx);
    SDL_GL_SetSwapInterval(1);

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

    int x, y;

    SDL_GetWindowSize(wallpaperWindowSDL, &x, &y);

    // Main loop

    bool control_window_shown = false;

    while (running) {

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL2_ProcessEvent(&e);
            switch (e.type)
            {
                case SDL_SYSWMEVENT:
                    if (e.syswm.msg->msg.win.msg == WM_USER + 1)
                    {
                        if (LOWORD(e.syswm.msg->msg.win.lParam) == WM_LBUTTONDOWN)
                        {
                            if (control_window_shown) {
                                SDL_HideWindow(control_window);
                            }
                            else {
                                SDL_ShowWindow(control_window);
                            }

                            control_window_shown = !control_window_shown;
                        }
                    }
                    break;
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }

        SDL_GL_MakeCurrent(control_window, control_gl_ctx);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("General Controls");
        if (ImGui::Button("Stop")) {
            running = false;
        }
        ImGui::End();

        ImGui::Render();

        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(control_window);

        SDL_GL_MakeCurrent(wallpaperWindowSDL, wallpaper_gl_ctx);

        glViewport(0, 0, x, y);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(wallpaperWindowSDL);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(control_gl_ctx);
    SDL_GL_DeleteContext(wallpaper_gl_ctx);

    Shell_NotifyIcon(NIM_DELETE, &icon);
    SDL_DestroyWindow(wallpaperWindowSDL);
    SDL_DestroyWindow(hidden_window);
    SDL_Quit();
    return 0;
}
