#include <iostream>
#include "Mixer.h"

Mixer::Mixer() {
    /*// Initialize SDL video and audio systems
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    // Initialize SDL mixer
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);

    Mix_AllocateChannels(8);

    // Create a SDL window
    m_window = SDL_CreateWindow("Hello, SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Mixer::width,
                                Mixer::height, SDL_WINDOW_OPENGL);

    // Create a renderer (accelerated and in sync with the display refresh rate)
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);

    m_sounds[SnareSound] = Mix_LoadWAV("../samples/Snare.wav");
    m_sounds[BassSound] = Mix_LoadWAV("../samples/Bass.wav");*/
}

Mixer::~Mixer() {
    /*SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    for (auto sound : m_sounds) {
        Mix_FreeChunk(sound.second);
    }
    Mix_CloseAudio();
    SDL_Quit();*/
}

void Mixer::play_sound(Sound sound) {
    // Load audio files
    cout << Mix_PlayChannel(-1, m_sounds[sound], 0) << endl;
    SDL_RenderClear(m_renderer);
    SDL_RenderPresent(m_renderer);
    // Clear screen
    //SDL_RenderClear(m_renderer);

    // Draw

    // Show what was drawn
    //SDL_RenderPresent(m_renderer);
    /*Mix_FreeChunk(m_sounds[sound]);
    if (sound == SnareSound) {
        m_sounds[SnareSound] = Mix_LoadWAV("../samples/Snare.wav");
        Mix_PlayChannel(0, m_sounds[sound], 0);
    } else if (sound == BassSound) {
        m_sounds[BassSound] = Mix_LoadWAV("../samples/Bass.wav");
        Mix_PlayChannel(1, m_sounds[sound], 0);
    }*/
}
