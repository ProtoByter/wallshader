#include "ControlWindow.hpp"
#include "SDL_syswm.h"
#include "resource.h"

ControlWindow::ControlWindow(HINSTANCE hInstance) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);

    atexit(SDL_Quit);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    this->window = SDL_CreateWindow("Control Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE);
    this->context = SDL_GL_CreateContext(this->window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    this->io = &ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(this->window, this->context);
    ImGui_ImplOpenGL3_Init(nullptr);

    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);

    if (SDL_GetWindowWMInfo(this->window, &info))
    {
        this->iconData.uCallbackMessage = WM_USER + 1;
        this->iconData.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_INFO;
        this->iconData.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
        this->iconData.cbSize = sizeof(this->iconData);
        this->iconData.hWnd = info.info.win.window;
        this->iconData.uVersion = NOTIFYICON_VERSION_4;
        strcpy_s(this->iconData.szTip, "WallShader");
        strcpy_s(this->iconData.szInfo, "Click to open the WallShader controls");
        strcpy_s(this->iconData.szInfoTitle, "WallShader");

        Shell_NotifyIcon(NIM_DELETE, &this->iconData);
        Shell_NotifyIcon(NIM_ADD, &this->iconData);
        Shell_NotifyIcon(NIM_SETVERSION, &this->iconData);
    }

    SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
}

bool ControlWindow::Update() {
    bool shouldClose;

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
                        this->control_window_shown = !this->control_window_shown;
                    }
                }
                break;
            case SDL_QUIT:
                this->control_window_shown = false;
                break;
        }
    }

    if (this->control_window_shown) {
        SDL_ShowWindow(this->window);
    }
    else {
        SDL_HideWindow(this->window);
    }

    SDL_GL_MakeCurrent(this->window, this->context);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("General Controls");

    shouldClose = ImGui::Button("Stop");

    ImGui::End();

    ImGui::Render();

    glViewport(0, 0, (int)io->DisplaySize.x, (int)io->DisplaySize.y);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(this->window);

    return shouldClose;
}

std::string ControlWindow::GetAddress() {
    std::string addr;
    addr.resize(32, '\0');
    sprintf_s(&addr[0], addr.size(), "%p", this->window);
    return addr;
}

ControlWindow::~ControlWindow() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    Shell_NotifyIcon(NIM_DELETE, &this->iconData);

    SDL_GL_DeleteContext(this->context);
    SDL_DestroyWindow(this->window);
}
