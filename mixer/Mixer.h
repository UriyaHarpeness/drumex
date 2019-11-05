#pragma once

#include <map>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "Sound.h"

using namespace std;


class Mixer {
public:
    static const int width = 100;

    static const int height = 100;

    Mixer();

    ~Mixer();

    void play_sound(Sound sound);

private:
    SDL_Window *m_window;

    SDL_Renderer *m_renderer;

    map<Sound, Mix_Chunk *> m_sounds;
};
