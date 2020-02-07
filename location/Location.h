#pragma once

#include "../notation/Notation.h"

namespace location {
    map<Fraction, Group> create_location_mapping(const Voice &voice);

    map<Fraction, Group> merge_locations(const vector<map<Fraction, Group>> &locations);

    Voice location_to_notation(map<Fraction, Group> &locations);
};
