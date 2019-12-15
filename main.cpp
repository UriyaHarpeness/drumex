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


void disp(shared_ptr<Display> &d) {
    char s[2];
    s[1] = 0;
    d->clear_screen();
    d->draw_text((MusicSymbolValues) 0xc0, 40 + (60), 100 + (60));
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
    Notation::m_display = d;
    /*Notation w(WholeNote);
    Notation h(HalfNote);
    Notation q(QuarterNote);
    Notation e(EightNote);
    for (int p = -9; p < 10; p++) {
        w.display(20 + 40, 100, p, 9 + p);
        h.display(20 + 40, 200, p, 9 + p);
        q.display(20 + 40, 300, p, 9 + p);
        e.display(20 + 40, 400, p, 9 + p);
    }*/

    //Notation t(BasePlay, SnareInst, {1, 8}, {ModAccent});
    //t.display(100, 100, 10);

    vector<vector<Notation>> notations = {{{BasePlay, SnareInst, {1, 16}, {ModAccent}}},
                                          {{BasePlay, ChinaInst, {1, 8},  {ModAccent}}, {BasePlay, HighTomInst, {1, 8}, {}}},
                                          {{BaseStop, UnboundUp,   {1, 16}, {}}},
                                          {{BasePlay, HiHatInst, {1, 8},  {ModAccent}}}};
    Notation::draw_connected_notes(50, 100, 0, notations);

    d->present();
    this_thread::sleep_for(chrono::milliseconds(1000));
}

void gamelogic() {
    SDL_Event e;
    bool done = false;
    shared_ptr<Display> d(new Display());

    int a = 0;
    while (!done) {
        disp(d);
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
    Notation::generate_notation(a, {1, 2}, {4, 8}, {4, 4}, BasePlay);
    Notation::generate_notation(a, {1, 8}, {4, 8}, {4, 4}, BasePlay);
    Notation::generate_notation(a, {1, 8}, {1, 8}, {4, 4}, BasePlay);
    Notation::generate_notation(a, {1, 8}, {1, 16}, {4, 4}, BasePlay);
    Notation::generate_notation(a, {15, 8}, {5, 16}, {4, 4}, BasePlay);

    //return 0;

    // sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev

    gamelogic();

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