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

    const int line_x_offset = 20;

    const int line_content_padding = 60;

    const int displaying_init_x = line_x_offset + line_content_padding;

    const int displaying_max_x = window_width - displaying_init_x;

    const int displaying_init_y = 100;

    // todo: change spacing dynamically.
    const int staff_lines_spacing = 180;

    const int max_lines_displayed = 3;

    const Fraction minimal_supported_fraction = {1, 16};

    const int minimal_distance = 14;

    // todo: this isn't correct, it's supposed to be `4 * line_height`.
    const int staff_to_0 = 2 * line_height;

    const int direction_line = 2;

    const int default_stem_length = 7;

    const int min_stem_length = 3;

    const int connector_height = 7;

    const int polyrhythm_height_line_spacing = 4;

    const int polyrhythm_height = 30;

    const int polyrhythm_sides_padding = 10;

    const int polyrhythm_sides_offset = 5;

    // Ledger lines settings.
    const array<int, 4> ledger_lines = {-8, -6, 6, 8};
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
