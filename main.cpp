#include "notation/NotationDisplayUtils.h"
#include "reader/Exercise.h"
#include "reader/Part.h"
#include "resources_menu/ResourcesMenu.h"

int main(int argv, char *argc[]) {
    string part_path;
    string exercise_path;
    int index = -1;
    int tempo = 80;

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

    shared_ptr<Display> display(new Display());

    // If resource/part not specified, display a menu.
    if ((exercise_path.empty() and part_path.empty())) {
        auto resources_menu = ResourcesMenu(filesystem::directory_entry("resources"));

        bool done = false;
        SDL_Event event;
        while (!done && !resources_menu.selected().has_value()) {
            while (SDL_PollEvent(&event)) {
                ImGui_ImplSDL2_ProcessEvent(&event);
                if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) || (event.type == SDL_QUIT)) {
                    done = true;
                }
            }

            display->clear_screen();
            resources_menu.display();
            display->present();
        }

        if (resources_menu.selected().has_value()) {
            if (get<2>(resources_menu.selected().value()) == PART) {
                part_path = get<0>(resources_menu.selected().value());
            } else {
                exercise_path = get<0>(resources_menu.selected().value());
            }
            index = get<1>(resources_menu.selected().value());
        }
    }

    if ((exercise_path.empty() and part_path.empty()) || (index < 0) || (tempo < 1)) {
        Log(ERROR).Get() << "Missing arguments: [--part|--exercise] [--index] [--tempo (optional)]" << endl;
        return 1;
    }

    DisplayVariables display_variables;
    Part *chosen_part;
    Exercise *exercise = nullptr;

    if (!exercise_path.empty()) {
        exercise = new Exercise(exercise_path, index);
        chosen_part = &exercise->get_part();
    } else {
        chosen_part = new Part(part_path, index);
    }
    Log(INFO).Get() << "Using tempo: " << tempo << endl;

    chosen_part->notationize();

    NotationDisplayUtils::prepare_displayable_notation(chosen_part->get_up(), chosen_part->get_down(),
                                                       display_variables);

    Notation::m_display = display;

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
    // todo: improve the schemas of parts and exercises to be more readable and clear, accordingly update schemas.
    // todo: add more instruments and modifiers and update everywhere (docs included), like HiHatFoot, same for variations or anything that change the schemas.
    // todo: maybe add a GUI editing option in the future.
    // todo: support both one voice writing and two voice and conversion between them.
    // todo: enforce time signature's lower part is always a power of 2.
    // todo: configurable font size, window size, etc.

    /**
     * todo:
     * Refer to MuseScore to see the nice practice of trying to split into the measure even in polyrhythm.
     * The polyrhythm selection is still isn't perfect, for example, for 1/9 2/9 3/9 4/9, it will divide to 1/3 and
     *  again to 1/3, instead of just starting with 1/9.
     * Polyrhythm over upper modifiers (like accent) will collide.
     */

    return 0;

    // sudo apt-get install gcc g++ cmake libsdl2-dev libsdl2-ttf-dev
    // optional: libsdl2-image-dev libsdl2-mixer-dev libasound2-dev
    // brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer jsoncpp
}
