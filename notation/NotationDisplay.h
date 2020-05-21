#pragma once

#include "Notation.h"
#include "../Enums.h"
#include "../display/Display.h"
#include "../display/DisplayConfigurations.h"

using namespace std;

namespace NotationDisplay {
    array<int, 2> get_display_scope(const Voice &beamed);

    void draw_connectors(int x, int staff_y, int line, int length, int number,
                         int tail_length = DisplayConstants::default_stem_length);

    void draw_connected_notes(const int staff_y, const GlobalLocations &global_locations, Fraction offset,
                              const Voice &notations);

    void draw_individual_notes(const int staff_y, const GlobalLocations &global_locations, const Fraction &offset,
                               const Voice &notations);
};
