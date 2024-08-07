cmake_minimum_required(VERSION 3.28)
project(spincat)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_RC_FLAGS "/fo${CMAKE_BINARY_DIR}/resource.res")

include_directories(imgui-master)
include_directories(imgui-master/backends)
include_directories("giflib-5.2.1")
include_directories("EasyGifReader-master")

file(GLOB ims imgui-master/*.cpp)

add_executable(spincat WIN32 main.cpp
        ${ims}
        imgui-master/backends/imgui_impl_opengl2.cpp
        imgui-master/backends/imgui_impl_win32.cpp
        giflib-5.2.1/dgif_lib.c
        giflib-5.2.1/egif_lib.c
        giflib-5.2.1/getarg.c
        giflib-5.2.1/gifalloc.c
        giflib-5.2.1/gifclrmp.c
        giflib-5.2.1/gif_err.c
        giflib-5.2.1/gif_font.c
        giflib-5.2.1/gif_hash.c
        giflib-5.2.1/openbsd-reallocarray.c
        giflib-5.2.1/qprintf.c
        giflib-5.2.1/quantize.c


        EasyGifReader-master/EasyGifReader.cpp
        src/utils/window.cpp
        src/utils/window.h
        src/catWindow/catWindow.cpp
        src/catWindow/catWindow.h
        src/utils/runtime.cpp
        src/utils/runtime.h
        src/resource.rc
        src/gl/gl.cpp
        src/gl/gl.h
        src/defines.h
        src/utils/gif.cpp
        src/utils/gif.h
        src/utils/config.cpp
        src/utils/config.h)

target_sources(spincat PRIVATE src/resource.rc)
target_link_libraries(spincat PUBLIC opengl32)