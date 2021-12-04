#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "ControlWindow.hpp"
#include "WallpaperWindow.hpp"

#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int ShowCmd) {

    ControlWindow control_window(hInstance);
    WallpaperWindow wallpaper_window(control_window.GetAddress().c_str());

    bool shouldClose = false;

    while (!shouldClose) {
        shouldClose |= control_window.Update();
        shouldClose |= wallpaper_window.Update();
    }
    return 0;
}
