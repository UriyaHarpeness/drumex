#include <SDL2/SDL.h>

#include "display/Display.h"
#include "notation/Notation.h"
#include "reader/Part.h"
#include "reader/Exercise.h"

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


void disp(const vector<vector<vector<Notation>>> &notations, TimeSignature signature) {
    Notation::display_notation(notations, signature);
    this_thread::sleep_for(chrono::milliseconds(1000));
}

void gamelogic(const vector<vector<vector<Notation>>> &notations, TimeSignature signature) {
    SDL_Event e;
    bool done = false;

    int a = 0;
    while (!done) {
        disp(notations, signature
        );
        a++;
        cout << a << endl;

        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                done = true;
            }
        }
        if (a == 20) break;
        this_thread::sleep_for(chrono::milliseconds(200)
        );
    }
}

int main() {
    shared_ptr<Display> d(new Display());
    Notation::m_display = d;

    Exercise exercise("../resources/exercises/vars.json", 0);
    gamelogic(exercise.get_parts()[0][0].get_notation(), exercise.get_parts()[0][0].get_signature());

    // Part part("../resources/variations/2s-variations.json", 3);
    // gamelogic(part.get_notation(), part.get_signature());

    // todo: support when supplying -1 or something to part to just concat all options.
    // todo: support merging exercises together or somethings for 3/4 and stuff.

    return 0;


    //what if 0 is given as the numerator? like 0/4

    vector<vector<Notation>> stuff = {
            {{BasePlay, BassInst,    {1, 8},  {}},              {BasePlay, HiHatInst, {1, 8}, {}}},
            {{BasePlay, HiHatInst,   {1, 8},  {}}},
            {{BasePlay, SnareInst,   {1, 8},  {ModCrossStick}}, {BasePlay, HiHatInst, {1, 8}, {}}},
            {{BasePlay, HiHatInst,   {1, 8},  {ModOpen}}},
            {{BaseRest, UnboundUp,   {1, 16}, {}}},
            {{BasePlay, HighTomInst, {1, 16}, {ModFlam}}},
            {{BasePlay, HighTomInst, {1, 16}, {ModDrag}}},
            {{BaseRest, UnboundUp,   {1, 16}, {}}},
            {{BasePlay, HighTomInst, {1, 16}, {ModFlam}}},
            {{BaseRest, UnboundUp,   {1, 16}, {}}},
            {{BaseRest, UnboundUp,   {1, 16}, {}}},
            {{BasePlay, HighTomInst, {1, 16}, {ModDrag}}},
            {{BaseRest, UnboundUp,   {1, 1},  {}}},
            {{BaseRest, UnboundUp,   {1, 2},  {}}},
            {{BasePlay, SnareInst,   {1, 2},  {}},              {BasePlay, BassInst,  {1, 2}, {}}},
            {{BasePlay, BassInst,    {1, 8},  {}},              {BasePlay, HiHatInst, {1, 8}, {}}},
            {{BasePlay, HiHatInst,   {1, 8},  {}}},
            {{BasePlay, SnareInst,   {1, 8},  {ModCrossStick}}, {BasePlay, HiHatInst, {1, 8}, {}}},
            {{BasePlay, HiHatInst,   {1, 8},  {ModOpen}}},
            {{BaseRest, UnboundUp,   {1, 16}, {}}},
            {{BasePlay, HighTomInst, {1, 16}, {ModFlam}}},
            {{BasePlay, HighTomInst, {1, 16}, {ModDrag}}},
            {{BaseRest, UnboundUp,   {1, 16}, {}}},
            {{BasePlay, HighTomInst, {1, 16}, {ModFlam}}},
            {{BaseRest, UnboundUp,   {1, 16}, {}}},
            {{BaseRest, UnboundUp,   {1, 16}, {}}},
            {{BasePlay, HighTomInst, {1, 16}, {ModDrag}}},
    };

    stuff = {
            {{BasePlay, SnareInst, {1, 8}, {ModRight}}, {BasePlay, BassInst, {1, 8}, {}}},
            {{BasePlay, SnareInst, {1, 8}, {ModLeft}}},
            {{BasePlay, SnareInst, {1, 8}, {ModRight}}, {BasePlay, BassInst, {1, 8}, {}}},
            {{BasePlay, SnareInst, {1, 8}, {ModRight}}, {BasePlay, BassInst, {1, 8}, {}}},
            {{BasePlay, SnareInst, {1, 8}, {ModLeft}}},
            {{BasePlay, SnareInst, {1, 8}, {ModRight}}, {BasePlay, BassInst, {1, 8}, {}}},
            {{BasePlay, SnareInst, {1, 8}, {ModLeft}}},
            {{BasePlay, SnareInst, {1, 8}, {ModLeft}}},
    };

    // todo: need to separate sounds and display individually and together at the same time...
    // todo: maybe limit the note length to single beat optionally.
    // todo: add support for buzz roll and maybe even open roll.
    // todo: full note rests on the low voice are outside the line.
    // todo: must support polyrhythm later.

    TimeSignature sig = {4, 4};

    // splitting voices from one voice notation, will support both one voice writing and two voice and conversion
    // between them.

    vector<vector<vector<Notation>>> notation = Notation::generate_notation(stuff, sig);

    // sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev

    gamelogic(notation, sig);

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

    unique_ptr<Exercise> x(new Exercise("/home/uriya/CLionProjects/drumex/resources/exercises/double-paradidles.json"));
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