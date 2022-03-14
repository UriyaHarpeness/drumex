#include "notation/NotationDisplayUtils.h"
#include "reader/Exercise.h"
#include "reader/Part.h"

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
    Part *chosen_part;
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
