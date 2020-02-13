#include "Display.h"

Display::Display() {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    m_window = SDL_CreateWindow("DrumEX", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
                                SDL_WINDOW_SHOWN);
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    m_screen = SDL_GetWindowSurface(m_window);

    m_music_font = TTF_OpenFont("../Drumex.ttf", 39);
    m_text_font = TTF_OpenFont("../WallingtonRegular-PYK7.ttf", 39);
}

Display::~Display() {
    // todo: free all the pointers..., there are many more across the sdl code.
    free(m_renderer);
    free(m_screen);
    free(m_window);
    free(m_music_font);
    free(m_text_font);
}

void Display::clear_screen() {
    SDL_SetRenderDrawColor(m_renderer, 222, 222, 222, 255);
    SDL_RenderClear(m_renderer);
}

void Display::draw_text(const string &text, int x, int y) {
    SDL_Surface *surface;
    SDL_Color textColor = {0, 0, 0, 0};

    surface = TTF_RenderText_Solid(m_music_font, text.c_str(), textColor);
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
    surface = TTF_RenderText_Solid(m_music_font, text, textColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect rect{x, y, surface->w, surface->h};
    SDL_RenderCopy(m_renderer, texture, nullptr, &rect);
}

void Display::draw_text(MusicSymbolValues value, int x, int staff_y, int line, int off_x, int off_y) {
    SDL_Surface *surface;
    SDL_Color textColor = {0, 0, 0, 0};

    char text[2] = {0, 0};
    text[0] = value;
    surface = TTF_RenderText_Solid(m_music_font, text, textColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect rect{x + off_x, staff_y + (line * line_height) + off_y, surface->w, surface->h};
    SDL_RenderCopy(m_renderer, texture, nullptr, &rect);
}

void Display::draw_text_c(const string &text, int x, int y) {
    SDL_Surface *surface;
    SDL_Color textColor = {0, 0, 0, 0};

    surface = TTF_RenderText_Solid(m_music_font, text.c_str(), textColor);
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

void Display::draw_rect(int x, int y, int h, int w, int r, int b, int g, int a) {
    SDL_Rect sdl_rect{x, y, w, h};
    SDL_SetRenderDrawColor(m_renderer, r, g, b, 255);
    SDL_RenderFillRect(m_renderer, &sdl_rect);
}

void Display::draw_rect_c(int x, int y, int h, int w, int gray_scale = 0) {
    SDL_Rect sdl_rect{x - (w / 2), y - (h / 2), w, h};
    SDL_SetRenderDrawColor(m_renderer, gray_scale, gray_scale, gray_scale, 255);
    SDL_RenderFillRect(m_renderer, &sdl_rect);
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
