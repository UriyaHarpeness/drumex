#pragma once

#include "../notation/Notation.h"

typedef map<Fraction, Group> Locations;

namespace location {
    Locations notation_to_location(const Voice &voice);

    Locations merge_locations(const vector<Locations> &locations);

    void stretch_locations(Locations &locations, const Fraction &final_length);

    map<Fraction, Locations> get_ratios(const Locations &locations);

    vector<Locations> split_voices_locations(const Locations &locations);

    Voice location_to_notation(Locations &locations);
};
