#pragma once

#include "../notation/Fraction.h"

#include <map>
#include <array>

using namespace std;

typedef array<int, 2> Padding;
typedef map<Fraction, Padding> Paddings;
// A mapping of absolute offset to the location and its padding.
typedef map<Fraction, pair<int, Padding>> GlobalLocations;

namespace DisplayConstants {
    const int window_height = 600;

    const int window_width = 900;

    const int line_height = 5;

    // todo: maybe extract all of these configurations somewhere, and avoid passing to external functions.
    const int connector_height = 7;

    const int displaying_init_x = 80;

    const int displaying_max_x = window_width - 80;

    const int displaying_init_y = 100;

    const int staff_lines_spacing = 120;

    const int max_lines_displayed = 4;

    const Fraction minimal_supported_fraction = {1, 16};

    const int minimal_distance = 14;

    const int staff_to_0 = 2 * line_height;

    const int direction_line = 2;
}

typedef struct DisplayVariables_s {
    int current_line;
    int start_line;
    int end_line;
    int start_bar;
    int end_bar;

    Paddings merged_padding;
    GlobalLocations global_locations;
    vector<int> bars_split;
} DisplayVariables;
