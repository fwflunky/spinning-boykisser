//
// Created by user on 07.08.2024.
//

#include "catWindow.h"
#include "../utils/window.h"
#include "../resource.h"
#include "../gl/gl.h"
#include "../utils/config.h"

bool catWindow::init(HINSTANCE__ *const hInstance, int const nCmdShow) {
    WNDCLASS const wc = {
        .style = CS_OWNDC,
        .lpfnWndProc   = wndProc,
        .hInstance     = hInstance,
        .lpszClassName = progTitle.c_str(),
    };

    RegisterClass(&wc);

    windowHandle = CreateWindowEx(
        WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_LAYERED | WS_EX_TRANSPARENT,
        progTitle.c_str(),
        (progTitle + " Window").c_str(),
        WS_POPUP | WS_VISIBLE,
        0, 0, WIN_X, WIN_Y,
        nullptr, nullptr, hInstance, nullptr
    );

    if(!windowHandle)
        return false;

    if(!setupTray()) {
        UnregisterClassA(progTitle.c_str(), hInstance);
        return false;
    }

    SetWindowLong(windowHandle, GWL_EXSTYLE, GetWindowLong(windowHandle, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);
    SetParent(windowHandle, utils::window::getDesktopWindow());

    show = nCmdShow;

    return true;
}

bool catWindow::setupTray() {
    nidHandle = {
        .cbSize = sizeof(NOTIFYICONDATA),
        .hWnd = windowHandle,
        .uID = 1,
        .uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP,
        .uCallbackMessage = WM_USER + 1,
        .hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_MYICON)),
    };
    lstrcpy(nidHandle.szTip, progTitle.data());

    return Shell_NotifyIconA(NIM_ADD, &nidHandle);
}

void catWindow::runAndShow() {
    ShowWindow(windowHandle, show);
    SetLayeredWindowAttributes(windowHandle, RGB(0, 255, 0), 0, LWA_COLORKEY);

    gl::init(windowHandle);
    SetTimer(windowHandle, 1, 40, [](HWND__ *const hwnd, UINT const uMsg, UINT_PTR const idEvent, DWORD const dwTime) {
        gl::update();
    });

    utils::config::load();

    MoveWindow(windowHandle, utils::config::getWinX(), utils::config::getWinY(), WIN_X, WIN_Y, false);
    gl::colorMode = utils::config::getColorMode();
    gl::nextColor(true);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    KillTimer(windowHandle, 1);

    RECT rect{};
    GetWindowRect(windowHandle, &rect);
    utils::config::save(rect.left, rect.top, gl::colorMode);

    gl::destroy();

    DestroyWindow(windowHandle);
    UnregisterClass(progTitle.c_str(), GetModuleHandle(nullptr));
    Shell_NotifyIcon(NIM_DELETE, &nidHandle);
}

LRESULT CALLBACK catWindow::wndProc(HWND__ *const hwnd, UINT const uMsg, WPARAM const wParam, LPARAM const lParam) {
    switch (uMsg)
    {
        case WM_USER + 1: {
            if (lParam == WM_RBUTTONUP) {
                addSelectors();
                return 1;
            }
            return 0;
        }
        case WM_COMMAND: {
            return handleSelection(LOWORD(wParam)) ? 1 : 0;
        }

        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}


void catWindow::addSelectors(){
    POINT pt;
    GetCursorPos(&pt);
    SetForegroundWindow(windowHandle);
    auto const hMenu = CreatePopupMenu();
    AppendMenu(hMenu, MF_STRING, 1, "Center");
    AppendMenu(hMenu, MF_STRING, 2, "Right Bottom");
    AppendMenu(hMenu, MF_STRING, 3, "Right Top");
    AppendMenu(hMenu, MF_STRING, 4, "Left Bottom");
    AppendMenu(hMenu, MF_STRING, 5, "Left Top");
    AppendMenu(hMenu, MF_SEPARATOR, 7, nullptr);
    AppendMenu(hMenu, MF_STRING, 9, "Next Color");
    AppendMenu(hMenu, MF_SEPARATOR, 8, nullptr);
    AppendMenu(hMenu, MF_STRING, 6, "Exit");

    TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, windowHandle, nullptr);
    DestroyMenu(hMenu);
}

bool catWindow::handleSelection(int const id) {
    switch (id)
    {
        case 1: {
            //center
            MoveWindow(windowHandle, (GetSystemMetrics(SM_CXSCREEN) / 2) - (256 / 2), (GetSystemMetrics(SM_CYSCREEN) / 2) - (240 / 2), WIN_X, WIN_Y, false);
            break;
        }

        case 2: {
            //rb
            MoveWindow(windowHandle, GetSystemMetrics(SM_CXSCREEN) - 256,  GetSystemMetrics(SM_CYSCREEN) - 240, WIN_X, WIN_Y, false);
            break;
        }
        case 3: {
            //rt
            MoveWindow(windowHandle, GetSystemMetrics(SM_CXSCREEN) - 256,  0, WIN_X, WIN_Y, false);
            break;
        }
        case 4: {
            //lb
            MoveWindow(windowHandle, 0,  GetSystemMetrics(SM_CYSCREEN) - 240, WIN_X, WIN_Y, false);
            break;
        }
        case 5: {
            //lt
            MoveWindow(windowHandle, 0,  0, WIN_X, WIN_Y, false);
            break;
        }
        case 6: {
            PostQuitMessage(0);
            break;
        }
        case 9: {
            gl::nextColor();
            break;
        }
        default:
            return false;
    }
    return true;
}