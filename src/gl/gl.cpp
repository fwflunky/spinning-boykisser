//
// Created by user on 07.08.2024.
//

#include <windows.h>

#define _USE_MATH_DEFINES
#include <cmath>

#include <gl/GL.h>
#include "gl.h"
#include "imgui_impl_opengl2.h"
#include "imgui_impl_win32.h"
#include "../defines.h"
#include "../utils/gif.h"

void gl::init(HWND__ *const hwnd) {
    windowHandle = hwnd;
    createDevice();

    wglMakeCurrent(windowData.hDC, hRC);

    initImGui();
    utils::gif::prepareData();
}

void gl::initImGui() {
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_InitForOpenGL(windowHandle);
    ImGui_ImplOpenGL2_Init();

    ImFontConfig font_config;
    font_config.OversampleH = 1;
    font_config.OversampleV = 1;
    font_config.PixelSnapH = true;

    static const ImWchar ranges[] = {0x0020, 0x00FF, 0x0400, 0x044F, 0};
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 14.0f, &font_config, ranges);
}

void gl::createDevice() {
    auto const hDc = GetDC(windowHandle);
    PIXELFORMATDESCRIPTOR constexpr pfd = {
        .nSize = sizeof(PIXELFORMATDESCRIPTOR),
        .nVersion = 1,
        .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        .iPixelType = PFD_TYPE_RGBA,
        .cColorBits = 32,
        .cAlphaBits = 8,
        .cDepthBits = 24,
        .iLayerType = PFD_MAIN_PLANE,
    };

    SetPixelFormat(hDc, ChoosePixelFormat(hDc, &pfd), &pfd);
    ReleaseDC(windowHandle, hDc);

    windowData.hDC = GetDC(windowHandle);
    hRC = wglCreateContext(windowData.hDC);
}

void gl::update() {
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();
    renderInternal();
    ImGui::EndFrame();

    ImGui::Render();

    glViewport(0, 0, WIN_X, WIN_Y);
    glClearColor(0, 255, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    SwapBuffers(windowData.hDC);
}

void gl::renderInternal() {
    ImDrawList *draw_list = ImGui::GetBackgroundDrawList();

    int memOffset = 0;
    auto const uRaw = utils::gif::nextFrame();

    for (int cH = 0; cH < WIN_Y; cH++) {
        for (int cW = 0; cW < WIN_X; cW++) {
            if (uRaw.size() > memOffset) {
                std::uint32_t col = uRaw[memOffset++];
                if(auto const colorBytes = (unsigned char*) &col; colorBytes[3] != 0x00) { //if pixel is not empty space
                    col = *(std::uint32_t*) &color;
                }
                draw_list->AddRectFilled({static_cast<float>(cW), static_cast<float>(cH)}, {1 + static_cast<float>(cW), 1 + static_cast<float>(cH)}, col);
            }
        }
    }
    tickRGB();
}

void gl::tickRGB() {
    if(colorMode == 5) {
        static float hue = 0.0f;
        hue += 0.001;
        if (hue > 1.0f)
            hue -= 1.0f;

        color[0] = (unsigned char) (std::fabs(std::cos(hue * 2 * M_PI)) * 255.0f);
        color[1] = (unsigned char) (std::fabs(std::cos(hue * 2 * M_PI - 2 * M_PI / 3)) * 255.0f);
        color[2] = (unsigned char) (std::fabs(std::cos(hue * 2 * M_PI - 4 * M_PI / 3)) * 255.0f);
    }
}

void gl::nextColor() {
    if(colorMode++ > 4)
        colorMode = 0;

    switch (colorMode) {
        case 0: { //red
            unsigned char constexpr ncolor[4] = {0xff, 0x00, 0x00, 0xff};
            memcpy(color, ncolor, 4);
            break;
        }
        case 1: { //green
            unsigned char constexpr ncolor[4] = {0x00, 0xfe, 0x00, 0xff};
            memcpy(color, ncolor, 4);
            break;
        }
        case 2: { //blue
            unsigned char constexpr ncolor[4] = {0x00, 0x00, 0xff, 0xff};
            memcpy(color, ncolor, 4);
            break;
        }
        case 3: { //white
            unsigned char constexpr ncolor[4] = {0xff, 0xff, 0xff, 0xff};
            memcpy(color, ncolor, 4);
            break;
        }
        case 4: { //black
            unsigned char constexpr ncolor[4] = {0x00, 0x00, 0x00, 0xff};
            memcpy(color, ncolor, 4);
            break;
        }
        case 5: { //rgb
            unsigned char constexpr ncolor[4] = {0xff, 0x00, 0x00, 0xff};
            memcpy(color, ncolor, 4);
            break;
        }
        default:
            break;
    }
}

void gl::destroy() {
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    wglMakeCurrent(nullptr, nullptr);
    ReleaseDC(windowHandle, windowData.hDC);
    wglDeleteContext(hRC);
}