#include <SDL2/SDL.h>

#include "display/Display.h"
#include "notation/Notation.h"
#include "notation/NotationUtils.h"
#include "notation/NotationDisplayUtils.h"
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


int main(int argv, char *argc[]) {
    string part_path;
    string exercise_path;
    int index = -1;
    int tempo = -1;

    for (int argument_index = 1; argument_index < argv; argument_index += 2) {
        if ("--part" == string(argc[argument_index])) {
            part_path = argc[argument_index + 1];
        } else if ("--exercise" == string(argc[argument_index])) {
            exercise_path = argc[argument_index + 1];
        } else if ("--index" == string(argc[argument_index])) {
            index = stoi(argc[argument_index + 1]);
        } else if ("--tempo" == string(argc[argument_index])) {
            tempo = stoi(argc[argument_index + 1]);
        }
    }

    if ((exercise_path.empty() and part_path.empty()) || (index < 0) || (tempo < 1)) {
        Log(ERROR).Get() << "Missing arguments: [--part|--exercise] [--index] [--tempo]" << endl;
        return 1;
    }

    DisplayVariables display_variables;
    Part *chosen_part = nullptr;
    Exercise *exercise = nullptr;

    if (!exercise_path.empty()) {
        exercise = new Exercise(exercise_path, index);
        chosen_part = &exercise->get_part();
    } else {
        chosen_part = new Part(part_path, index);
    }

    chosen_part->notationize();

    NotationDisplayUtils::prepare_displayable_notation(chosen_part->get_up(), chosen_part->get_down(),
                                                       display_variables);

    shared_ptr<Display> d(new Display());
    Notation::m_display = d;

    NotationDisplayUtils::continuous_display_notation(chosen_part->get_up(), chosen_part->get_down(), display_variables,
                                                      tempo);

    if (exercise == nullptr) {
        delete chosen_part;
    } else {
        delete exercise;
    }

    // todo: maybe limit the note length to single beat optionally.
    // todo: add support for buzz roll and maybe even open roll.
    // todo: add option to enable dotted rests.

    /**
     * todo:
     * Refer to MuseScore to see the nice practice of trying to split into the measure even in polyrhythm.
     * The polyrhythm selection is still isn't perfect, for example, for 1/9 2/9 3/9 4/9, it will divide to 1/3 and
     *  again to 1/3, instead of just starting with 1/9.
     * Cymbal whole and half note head is like a bold but hollow X by MuseScore, need to add the head.
     * Polyrhythm over upper modifiers (like accent) will collide.
     */

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
     */

    // splitting voices from one voice notation, will support both one voice writing and two voice and conversion
    // between them.

    // sudo apt-get install gcc g++ cmake libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
    // libasound2-dev possibly, if will use sound.
}
