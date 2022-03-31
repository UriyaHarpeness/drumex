cmake_minimum_required(VERSION 3.6)
add_library(imgui STATIC
        imgui_impl_opengl3.cpp
        imgui_impl_opengl3.h
        imgui_impl_opengl3_loader.h
        imgui_impl_sdl.cpp
        imgui_impl_sdl.h
        imconfig.h
        imgui.cpp
        imgui.h
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_internal.h
        imgui_tables.cpp
        imgui_widgets.cpp
        imstb_rectpack.h
        imstb_textedit.h
        imstb_truetype.h
        )

# todo: find dynamically
target_include_directories(imgui PUBLIC /usr/local/include/SDL2/)
target_link_libraries(imgui /usr/local/lib/libSDL2.dylib /usr/local/lib/libSDL2main.a)
