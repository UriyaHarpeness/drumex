#pragma once

#include "Fraction.h"
#include "Notation.h"
#include "NotationUtils.h"
#include "../Enums.h"
#include "../display/Display.h"
#include "../metronome/Metronome.h"

#include <map>
#include <numeric>
#include <cassert>
#include <algorithm>

using namespace std;

namespace NotationDisplay {
    const int connector_height = 7;

    const int displaying_init_x = 50;

    const int displaying_init_y = 100;

    const int staff_lines_spacing = 100;

    const int max_lines_displayed = 4;

    const Fraction minimal_supported_fraction = {1, 16};

    void draw_connectors(int x, int staff_y, int line, int length, int number, int tail_length = 7);

    int calc_needed_space(const Paddings &distances, const Fraction &offset, const Fraction &length);

    void draw_connected_notes(int &x, int staff_y, const Paddings &distances, Fraction offset, const Voice &notations);

    void
    draw_individual_notes(int &x, int staff_y, const Paddings &distances, const Fraction &offset, const Group &group);

    /**
     * Display the notation.
     *
     * @param notation  The notation do display, its type is complicated and here's why, it's a vector:
     *                  1.  of single notes.
     *                  2.  of grouped notes (played at the same time with same length).
     *                  3.  of connected notes (notes that are beamed tigether).
     *                  4.  of voices.
     * @param signature The time signature.
     */
    void display_notation(const vector<vector<vector<vector<Notation>>>> &notation, const Paddings &distances,
                          const Fraction &bar, int played_line);

    pair<pair<int, int>, Padding> get_note_location(const GroupedNotations &notation, const Paddings &distances,
                                                    const Fraction &bar, const Fraction &location);

    void continuous_display_notation(const GroupedNotations &notation, const Paddings &distances, const Fraction &bar,
                                     int tempo);

    void
    prepare_displayable_notation(const Notations &generated_notation, GroupedNotations &notation, Paddings &distances,
                                 const TimeSignature &bar);

    Padding get_distance(const Fraction &length, Padding padding);
};
