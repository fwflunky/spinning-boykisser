//
// Created by user on 07.08.2024.
//

#ifndef GL_H
#define GL_H

#include <windows.h>

class gl {
public:
    static void init(HWND__ * hwnd);
    static void update();
    static void destroy();

    static void nextColor();
private:
    struct WGL_WindowData {
        HDC hDC;
    };

    static void createDevice();
    static void initImGui();
    static void renderInternal();
    static void tickRGB();

    static inline unsigned char color[4] = {0x00, 0x00, 0x00, 0xff}; //black
    static inline unsigned char colorMode = 4;

    static inline WGL_WindowData windowData;
    static inline HWND__ *windowHandle = nullptr;
    static inline HGLRC hRC = nullptr;
};



#endif //GL_H
