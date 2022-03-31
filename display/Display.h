#pragma once

#include "../Enums.h"
#include "DisplayConfigurations.h"

#include <iostream>
#include <map>
#include <thread>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else

#include <SDL_opengl.h>

#endif

using namespace std;

class Display {
public:
    Display();

    ~Display();

    pair<int, int> get_window_size();

    void clear_screen();

    string resolve_non_ascii(const string &s);

    void draw_text(const string &text, int x, int y);

    void draw_text(MusicSymbols value, int x, int y);

    void draw_text(MusicSymbols value, int x, int staff_y, int line, int off_x = 0, int off_y = 0);

    void draw_text_c(const string &text, int x, int y);

    pair<int, int> get_size(const string &text);

    void draw_rect(int x, int y, int h, int w, int gray_scale = 0);

    void draw_rect(int x, int y, int h, int w, int r, int g, int b, int a);

    void draw_rect_c(int x, int y, int h, int w, int gray_scale);

    void draw_base(int y, uint8_t numerator, uint8_t denominator);

    pair<int, int> reset_window_size(int maximum_bar_size);

    void present();

private:
    SDL_Window *m_window;
    ImFont *m_music_font;
    ImFont *m_text_font;  // todo: maybe use custom font.
    SDL_GLContext m_gl_context;
    ImGuiIO *m_io;
};
