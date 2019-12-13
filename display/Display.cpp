#include "Display.h"

Display::Display() {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    m_window = SDL_CreateWindow("Tultul", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
                                SDL_WINDOW_SHOWN);
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    m_screen = SDL_GetWindowSurface(m_window);
    m_font = TTF_OpenFont("../Petrucci.ttf", 39);
    //m_font = TTF_OpenFont("../Stencil.ttf", 42);
}

Display::~Display() {
    // todo: free all the pointers..., there are many more across the sdl code.
    free(m_renderer);
    free(m_screen);
    free(m_window);
    free(m_font);
}

void Display::clear_screen() {
    SDL_SetRenderDrawColor(m_renderer, 222, 222, 222, 255);
    SDL_RenderClear(m_renderer);
}

void Display::draw_text(const string &text, int x, int y) {
    SDL_Surface *surface;
    SDL_Color textColor = {0, 0, 0, 0};

    surface = TTF_RenderText_Solid(m_font, text.c_str(), textColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect rect{x, y, surface->w, surface->h};
    SDL_RenderCopy(m_renderer, texture, nullptr, &rect);
}

void Display::draw_text(MusicSymbolValues value, int x, int y) {
    SDL_Surface *surface;
    SDL_Color textColor = {0, 0, 0, 0};

    char text[2] = {0, 0};
    text[0] = value;
    surface = TTF_RenderText_Solid(m_font, text, textColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect rect{x, y, surface->w, surface->h};
    SDL_RenderCopy(m_renderer, texture, nullptr, &rect);
}

void Display::draw_text_c(const string &text, int x, int y) {
    SDL_Surface *surface;
    SDL_Color textColor = {0, 0, 0, 0};

    surface = TTF_RenderText_Solid(m_font, text.c_str(), textColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect rect{x - (surface->w / 2), y - (surface->h / 2), surface->w, surface->h};
    SDL_RenderCopy(m_renderer, texture, nullptr, &rect);
}

void Display::draw_rect(int x, int y, int h, int w, int gray_scale) {
    SDL_Rect sdl_rect{x, y, w, h};
    SDL_SetRenderDrawColor(m_renderer, gray_scale, gray_scale, gray_scale, 255);
    SDL_RenderFillRect(m_renderer, &sdl_rect);
}

void Display::draw_rect_c(int x, int y, int h, int w, int gray_scale = 0) {
    SDL_Rect sdl_rect{x - (w / 2), y - (h / 2), w, h};
    SDL_SetRenderDrawColor(m_renderer, gray_scale, gray_scale, gray_scale, 255);
    SDL_RenderFillRect(m_renderer, &sdl_rect);
}

void Display::draw_time_signature(uint8_t a, uint8_t b) {
    draw_text(to_string(a), 80, Display::staff_start + 17);
    draw_text(to_string(b), 80, Display::staff_start + 57);
}

void Display::draw_clef() {
    draw_rect_c(32, Display::staff_start + 5, 6, 16);
    draw_rect_c(22, Display::staff_start + 40, 76, 3);
    draw_rect_c(32, Display::staff_start + 75, 6, 16);
    draw_rect_c(38, Display::staff_start + 40, 76, 3);
}

void Display::draw_staff() {
    // todo: later needs support several staffs for long compositions
    for (int i = 0; i < 5; i++) {
        draw_rect_c(Display::width / 2, (i * 20) + Display::staff_start, 2, Display::width - 20, 126);
    }
}

void Display::draw_ending() {
    draw_rect_c(Display::width - 16, Display::staff_start + 40, 80, 12);
    draw_rect_c(Display::width - 30, Display::staff_start + 40, 80, 2);
}

void Display::draw_base_bak(uint8_t a, uint8_t b) {
    draw_staff();
    draw_clef();
    draw_time_signature(3, 16);
    draw_ending();
}

void Display::draw_base(int x, int y, uint8_t a, uint8_t b) {
    draw_text("===================", x, y);
    draw_text(SymClef, x + 10, y - 9);
}

void Display::draw_image(const string &path, int x, int y, int h, int w) {
    SDL_Texture *img = IMG_LoadTexture(m_renderer, path.c_str());
    //SDL_QueryTexture(img, nullptr, nullptr, &w, &h); // get the width and height of the texture
    // put the location where we want the texture to be drawn into a rectangle
    // I'm also scaling the texture 2x simply by setting the width and height
    SDL_Rect texr = {x, y, w, h};

    SDL_RenderCopy(m_renderer, img, nullptr, &texr);
}

void Display::present() {
    SDL_RenderPresent(m_renderer);
}
