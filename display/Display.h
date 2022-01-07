#pragma once

#include "../Enums.h"
#include "DisplayConfigurations.h"

#include <iostream>
#include <map>
#include <thread>
#include <SDL_image.h>
#include <SDL_ttf.h>

using namespace std;

class Display {
public:
    Display();

    ~Display();

    void clear_screen();

    void draw_text(const string &text, int x, int y);

    void draw_text(MusicSymbols value, int x, int y);

    void draw_text(MusicSymbols value, int x, int staff_y, int line, int off_x = 0, int off_y = 0);

    void draw_text_c(const string &text, int x, int y);

    pair<int, int> get_size(const string &text);

    void draw_rect(int x, int y, int h, int w, int gray_scale = 0);

    void draw_rect(int x, int y, int h, int w, int r, int g, int b, int a);

    void draw_rect_c(int x, int y, int h, int w, int gray_scale);

    void draw_base(int x, int y, uint8_t a, uint8_t b);

    void draw_image(const string &path, int x, int y, int h, int w);

    void present();

private:
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    SDL_Surface *m_screen;
    TTF_Font *m_music_font;
    TTF_Font *m_text_font;
};
