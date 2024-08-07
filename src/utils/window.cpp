//
// Created by user on 07.08.2024.
//

#include "window.h"

HWND utils::window::getDesktopWindow() {
    HWND__ *const progman = FindWindow("ProgMan", nullptr);
    SendMessageTimeout(progman, 0x052C, 0, 0, SMTO_NORMAL, 1000, nullptr);
    HWND__ *wallpaper_hwnd = nullptr;

    EnumWindows([](HWND__ *const hwnd, LPARAM const lParam) -> BOOL {
        const auto ret = (HWND__**) lParam;
        if(FindWindowEx(hwnd, nullptr, "SHELLDLL_DefView", nullptr)) {
            *ret = FindWindowEx(nullptr, hwnd, "WorkerW", nullptr);
        }
        return true;
    }, (LPARAM) &wallpaper_hwnd);

    return wallpaper_hwnd;
}