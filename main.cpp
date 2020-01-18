#include <SDL2/SDL.h>

#include "display/Display.h"
#include "notation/Notation.h"

int main2(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window *window = NULL;
        SDL_Renderer *renderer = NULL;

        if (SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer) == 0) {
            SDL_bool done = SDL_FALSE;

            while (!done) {
                SDL_Event event;

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawLine(renderer, 320, 200, 300, 240);
                SDL_RenderDrawLine(renderer, 300, 240, 340, 240);
                SDL_RenderDrawLine(renderer, 340, 240, 320, 200);
                SDL_RenderPresent(renderer);

                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        done = SDL_TRUE;
                    }
                }
            }
        }

        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
    }
    SDL_Quit();
    return 0;
}

/////////////////////////////////


void disp(const vector<vector<vector<Notation>>> &notations) {
    char s[2];
    s[1] = 0;
    const auto &d = Notation::m_display;
    d->clear_screen();
    //d.draw_base(3, 16);
    /*for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            s[0] = (char) (i * 16 + j);
            if (s[0] == 0)continue;

            //d.draw_image("../stuff.png", 400, 0, a * 8, a * 8);
            //d.draw_rect(a, a, 80, 10);

            string k(s);
            d->draw_text(k, 40 + (j * 60), 100 + (i * 60));
            //d.draw_text(to_string(i * 16 + j), 100, 100);
        }
    }*/

    int p;
    for (p = 1; p <= 4; p++) {
        d->draw_base(20, p * 100, 3, 4);
    }

    int off_x = 50;
    for (const vector<vector<Notation>> &note_groups : notations) {
        if (note_groups.size() > 1) {
            Notation::draw_connected_notes(off_x, 100, note_groups);
        } else {
            Notation::draw_individual_notes(off_x, 100, note_groups[0]);
        }
    }

    d->present();
    this_thread::sleep_for(chrono::milliseconds(1000));
}

void gamelogic(const vector<vector<vector<Notation>>> &notations) {
    SDL_Event e;
    bool done = false;

    int a = 0;
    while (!done) {
        disp(notations);
        a++;
        cout << a << endl;

        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                done = true;
            }
        }
        if (a == 20) break;
        this_thread::sleep_for(chrono::milliseconds(200));
    }
}

int main() {
    Action a(Regular, UnboundStick);
    // todo: dont use make_pair, use the Action constructor everywhere
    shared_ptr<Display> d(new Display());
    Notation::m_display = d;
    //Notation::generate_notation(a, {6, 16}, {2, 16}, {3, 8}, BasePlay);
    //what if 0 is given as the numerator? like 0/4

    vector<vector<Notation>> stuff = {
            {{BasePlay, FloorTomInst, {1, 8},  {}},              {BasePlay, HiHatInst, {1, 8}, {}}},
            {{BasePlay, HiHatInst,    {1, 8},  {}}},
            {{BasePlay, SnareInst,    {1, 8},  {ModCrossStick}}, {BasePlay, HiHatInst, {1, 8}, {}}},
            {{BasePlay, HiHatInst,    {1, 8},  {ModOpen}}},
            {{BaseRest, UnboundUp,    {1, 16}, {}}},
            {{BasePlay, HighTomInst,  {1, 16}, {}}},
            {{BasePlay, HighTomInst,  {1, 16}, {}}},
            {{BaseRest, UnboundUp,    {1, 16}, {}}},
            {{BasePlay, HighTomInst,  {1, 16}, {}}},
            {{BaseRest, UnboundUp,    {1, 16}, {}}},
            {{BaseRest, UnboundUp,    {1, 16}, {}}},
            {{BasePlay, HighTomInst,  {1, 16}, {}}},
            {{BaseRest, UnboundUp,    {1, 1},  {}}},
    };

    stuff = {
            {{BasePlay, BassInst, {1, 16}, {}}},
            {{BasePlay, BassInst, {1, 16}, {}}},
            {{BasePlay, BassInst, {1, 8},  {}}},
            {{BasePlay, BassInst, {3, 4},  {}}},
    };
    // todo: add whole note support here, and add dots.
    stuff = {
            {{BasePlay, SnareInst, {3, 8}, {}}},
            {{BasePlay, SnareInst, {1, 8}, {}}},
            {{BasePlay, SnareInst, {1, 2}, {}}},
    };
    // todo next: add dots in bars and display connected only where relevant.

    vector<vector<Notation>> merged_stuff = Notation::merge_notation(stuff);

    TimeSignature sig = {4, 4};
    Fraction beat = {1, sig.second};
    Fraction bar = {sig.first, sig.second};

    // todo: shouldn't really use beat, first split to bars and then beats.
    vector<vector<Notation>> notations = Notation::generate_notation(merged_stuff, sig);

    // seems problematic notations[2][0].m_symbol

    vector<vector<vector<Notation>>> splitted_notation = Notation::split_notation(notations, bar);

    vector<vector<vector<Notation>>> connected_notation;
    vector<vector<vector<Notation>>> small_connected_notation;

    for (const vector<vector<Notation>> &small_notation : splitted_notation) {
        small_connected_notation = Notation::connect_notation(small_notation, beat);
        connected_notation.insert(connected_notation.end(), small_connected_notation.begin(),
                                  small_connected_notation.end());
    }

    // sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev

    gamelogic(connected_notation);

    return 0;
}
/*#include <iostream>

#include "metronome/Metronome.h"
#include "role/Role.h"
#include "mixer/Mixer.h"
#include "reader/Reader.h"
#include "exercise/Exercise.h"

int main() {
    std::cout << "hi" << std::endl;

    //unique_ptr<Exercise> x(new Exercise("/home/uriya/CLionProjects/drumex/resources/exercises/apply-variations.json"));
    //vector<pair<string, int>> choice = {{"Paradidles.", 0}};

    //unique_ptr<Exercise> x(new Exercise("/home/uriya/CLionProjects/drumex/resources/exercises/polyrhythm-3-on-4.json"));
    //vector<pair<string, int>> choice = {{"3's Variations.",  3},
    //                                    {"4 On 4, 3 Notes.", 0}};

    unique_ptr<Exercise> x(new Exercise("/home/uriya/CLionProjects/drumex/resources/exercises/double-stuff.json"));
    vector<pair<string, int>> choice = {{"3's Variations.", 3}};

    vector<Role> roles = x->choose_exercises(choice);

    unique_ptr<Mixer> mixer(new Mixer());

    Metronome m(roles, 110, move(mixer));
    // add applyable change hands, like changing tempo and choose...

    m.start();

    // sudo apt install libasound2-dev needed for build.
    // sudo apt install libsdl2-dev
    // libsdl2-image-2.0-0/disco no, used libsdl2-image-dev
    // libsdl2-ttf-dev/disco
    // libsdl2-mixer-dev
    // gcc -o main main.c -lSDL2 -lSDL2_mixer

    while (true);
    return 0;
}
*/