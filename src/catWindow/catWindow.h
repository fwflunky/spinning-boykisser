//
// Created by user on 07.08.2024.
//

#ifndef CATWINDOW_H
#define CATWINDOW_H

#include <string>
#include <windows.h>
#include "../defines.h"

class catWindow {
public:
    static bool init(HINSTANCE__ *hInstance, int nCmdShow);
    static void runAndShow();

    static inline const std::string progTitle = PROG_TITLE;
private:
    static LRESULT CALLBACK wndProc(HWND__ *hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static void addSelectors();
    static bool setupTray();

    static bool handleSelection(int id);

    static inline HWND__ *windowHandle = nullptr;
    static inline NOTIFYICONDATA nidHandle;
    static inline int show = 0;

};



#endif //CATWINDOW_H
