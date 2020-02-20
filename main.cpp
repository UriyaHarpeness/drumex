#include <SDL2/SDL.h>

#include "display/Display.h"
#include "notation/Notation.h"
#include "notation/NotationUtils.h"
#include "notation/NotationDisplay.h"
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


int main() {
    shared_ptr<Display> d(new Display());
    Notation::m_display = d;

    // Exercise exercise("../resources/exercises/chester-2-a.json", 0);
    // const Part &part = exercise.get_part();

    Part part("../resources/play/playing.json", 0);

    GroupedNotations notation;
    Paddings distances;

    NotationDisplay::prepare_displayable_notation(part.get_notation(), notation, distances, part.get_signature());

    // todo: also support exiting through keyboard event and change tempo.
    NotationDisplay::continuous_display_notation(notation, distances, part.get_signature(), 180);

    // todo: need to print the time signature next to the clef.
    // todo: maybe limit the note length to single beat optionally.
    // todo: add support for buzz roll and maybe even open roll.
    // todo: add option to enable dotted rests.
    // todo: must support polyrhythm later.

    return 0;

    /*

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

    TimeSignature sig = {4, 4};

    // splitting voices from one voice notation, will support both one voice writing and two voice and conversion
    // between them.

    vector<vector<vector<Notation>>> notation = Notation::generate_notation(stuff, sig);

    // sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
    // libasound2-dev possibly, if will use sound.

    gamelogic(notation, sig);*/

    return 0;
}
