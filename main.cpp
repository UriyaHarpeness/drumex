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


void disp(const vector<vector<Notation>> &notations) {
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

    /*
    {
        Notation t(BasePlay, SnareInst, {1, 8}, {ModAccent});
        t.display(100, 200, true);
        Notation t2(BasePlay, SnareInst, {1, 16}, {ModAccent});
        t2.display(130, 200, true);
        Notation t3(BasePlay, SnareInst, {1, 32}, {ModAccent});
        t3.display(160, 200, true);
    }

    {
        Notation t(BasePlay, BassInst, {1, 8}, {ModAccent});
        t.display(200, 200, true);
        Notation t2(BasePlay, BassInst, {1, 16}, {ModAccent});
        t2.display(230, 200, true);
        Notation t3(BasePlay, BassInst, {1, 32}, {ModAccent});
        t3.display(260, 200, true);
        Notation t4(BasePlay, BassInst, {1, 64}, {ModAccent});
        t4.display(290, 200, true);
    }

    {
        // todo: check cymbals lower line.
        Notation t(BasePlay, FloorTomInst, {1, 8}, {ModAccent});
        t.display(300, 200, true);
        Notation t2(BasePlay, FloorTomInst, {1, 16}, {ModAccent});
        t2.display(330, 200, true);
        Notation t3(BasePlay, FloorTomInst, {1, 32}, {ModAccent});
        t3.display(360, 200, true);
    }

    // todo: support polyrhythm beaming.

    vector<vector<Notation>> notations = {{{BasePlay, SnareInst, {1, 16}, {ModAccent}}},
                                          {{BasePlay, SnareInst, {1, 16}, {}}},
                                          {{BasePlay, ChinaInst, {1, 16}, {ModAccent, ModDot}}, {BasePlay, HighTomInst, {1, 16}, {ModDot}}},
                                          {{BaseRest, UnboundUp, {1, 16}, {}}},
                                          {{BasePlay, SnareInst, {1, 16}, {}}},
                                          {{BasePlay, SnareInst, {1, 16}, {ModGhost}}},
                                          {{BasePlay, SnareInst, {1, 16}, {ModGhost}}},
                                          {{BasePlay, SnareInst, {1, 16}, {}}},
                                          {{BasePlay, SnareInst, {1, 16}, {ModDot}}},
                                          {{BasePlay, SnareInst, {1, 16}, {}}},
                                          {{BasePlay, SnareInst, {1, 16}, {ModCrossStick}}},
                                          {{BasePlay, SnareInst, {1, 16}, {ModCrossStick}}},
                                          {{BasePlay, SnareInst, {1, 16}, {}}},
                                          {{BasePlay, SnareInst, {1, 16}, {ModRimshot}}},
                                          {{BasePlay, SnareInst, {1, 16}, {ModRimshot}}},
                                          {{BasePlay, SnareInst, {1, 16}, {}}},
                                          {{BasePlay, SnareInst, {1, 16}, {ModDrag}}},
                                          {{BasePlay, SnareInst, {1, 16}, {ModDrag}}},
                                          {{BasePlay, SnareInst, {1, 16}, {}}},
                                          {{BasePlay, SnareInst, {1, 16}, {ModFlam}}},
                                          {{BasePlay, SnareInst, {1, 16}, {}}},
                                          {{BasePlay, HiHatInst, {1, 16}, {ModClose}}},
                                          {{BasePlay, SnareInst, {1, 16}, {ModFlam}},           {BasePlay, HighTomInst, {1, 16}, {ModGhost}}},
                                          {{BasePlay, HiHatInst, {1, 16}, {ModLoose}}},
                                          {{BasePlay, SnareInst, {1, 16}, {}}},
                                          {{BasePlay, HiHatInst, {1, 16}, {ModOpenClose}}},
                                          {{BasePlay, HiHatInst, {1, 16}, {ModOpenClose}}},
                                          {{BasePlay, SnareInst, {1, 16}, {}}},
                                          {{BasePlay, HiHatInst, {1, 16}, {ModOpen}}},
                                          {{BasePlay, SnareInst, {1, 16}, {}}},
                                          {{BasePlay, HiHatInst, {1, 16}, {ModChoke}}},
                                          {{BasePlay, HiHatInst, {1, 16}, {ModChoke}}},
                                          {{BasePlay, SnareInst, {1, 16}, {}}},
                                          {{BasePlay, HiHatInst, {1, 16}, {ModDot}}}};
    int off_x = 50;
    Notation::draw_connected_notes(off_x, 100, notations);
    cout << off_x << endl;*/

    int off_x = 50;
    Notation::draw_connected_notes(off_x, 100, notations);
    cout << off_x << endl;


    d->present();
    this_thread::sleep_for(chrono::milliseconds(1000));
}

void gamelogic(const vector<vector<Notation>> &notations) {
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
    /*Notation::generate_notation(a, {1, 2}, {4, 8}, {4, 4}, BasePlay);
    Notation::generate_notation(a, {1, 8}, {4, 8}, {4, 4}, BasePlay);
    Notation::generate_notation(a, {1, 8}, {1, 8}, {4, 4}, BasePlay);
    Notation::generate_notation(a, {1, 8}, {1, 16}, {4, 4}, BasePlay);
    Notation::generate_notation(a, {15, 8}, {5, 16}, {4, 4}, BasePlay);*/

    vector<vector<Notation>> stuff = {
            {{BasePlay, SnareInst, {1, 16}, {}}},
            {{BasePlay, SnareInst, {1, 8},  {ModRimshot}}},
            {{BasePlay, SnareInst, {1, 16}, {}}},
    };

    vector<vector<Notation>> merged_stuff = Notation::merge_notation(stuff);

    TimeSignature sig = {4, 4};
    vector<vector<Notation>> notations = Notation::generate_notation(merged_stuff, sig);

    //return 0;

    // sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev

    gamelogic(notations);

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