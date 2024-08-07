#include <iostream>
#include <windows.h>

#include "src/catWindow/catWindow.h"
#include "src/utils/runtime.h"

int WINAPI wWinMain(HINSTANCE__ *const hInstance, HINSTANCE__ *const hPrevInstance, wchar_t *const lpCmdLine, int const nShowCmd)
{
    if(utils::runtime::isAlreadyRunning()) {
        MessageBoxA(nullptr, (catWindow::progTitle + " already running").c_str(), catWindow::progTitle.c_str(), MB_ICONSTOP);
        return 1;
    }

    if(!utils::runtime::createRunLock()) {
        MessageBoxA(nullptr, (catWindow::progTitle + " failed to startup").c_str(), catWindow::progTitle.c_str(), MB_ICONSTOP);
        return 1;
    }

    if(!catWindow::init(hInstance, nShowCmd)) {
        MessageBoxA(nullptr, (catWindow::progTitle + " failed to initialize").c_str(), catWindow::progTitle.c_str(), MB_ICONSTOP);
        utils::runtime::destroyRunLock();
        return 1;
    }

    catWindow::runAndShow();
    utils::runtime::destroyRunLock();

    return 0;
}

