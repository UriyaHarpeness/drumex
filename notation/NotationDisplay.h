#pragma once

#include "Fraction.h"
#include "Notation.h"
#include "NotationUtils.h"
#include "../Enums.h"
#include "../display/Display.h"
#include "../metronome/Metronome.h"
#include "../notation_container/VoiceContainer.h"

#include <map>
#include <numeric>
#include <cassert>
#include <algorithm>

using namespace std;

namespace NotationDisplay {
    // todo: maybe extract all of these configurations somewhere, and avoid passing to external functions.
    const int connector_height = 7;

    const int displaying_init_x = 80;

    const int displaying_max_x = Display::width - 20;

    const int displaying_init_y = 100;

    const int staff_lines_spacing = 120;

    const int max_lines_displayed = 4;

    const Fraction minimal_supported_fraction = {1, 16};

    void draw_connectors(int x, int staff_y, int line, int length, int number, int tail_length = 7);

    int calc_needed_space(const Paddings &distances, const Fraction &offset, const Fraction &length);

    void draw_connected_notes(int &x, int staff_y, const Paddings &distances, Fraction offset, const Voice &notations);

    void
    draw_individual_notes(const int staff_y, const GlobalLocations &global_locations, const Fraction &offset,
                          const Group &group);

    /**
     * Display the notation.
     */
    void display_notation(const VoiceContainer &up, const VoiceContainer &down,
                          const Fraction &current_location, const vector<int> &bar_splits,
                          const GlobalLocations &global_locations);

    pair<pair<int, int>, Padding> get_note_location(const GroupedNotations &notation, const Paddings &distances,
                                                    const Fraction &bar, const Fraction &location);

    void prepare_displayable_notation(VoiceContainer &up, VoiceContainer &down, Paddings *merged_padding,
                                      GlobalLocations *global_locations, vector<int> &bars_split);

    void
    continuous_display_notation(const VoiceContainer &up, const VoiceContainer &down, const Paddings &merged_padding,
                                const GlobalLocations &global_locations, const vector<int> &bar_splits, int tempo);

    GlobalLocations
    create_global_locations(const Paddings &padding, vector<int> &bars_split, const TimeSignature &signature);

    Padding get_distance(const Fraction &length, Padding padding);
};
