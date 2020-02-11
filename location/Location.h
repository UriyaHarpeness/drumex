#pragma once

#include "../notation/Notation.h"

namespace location {
    map<Fraction, Group> notation_to_location(const Voice &voice);

    map<Fraction, Group> merge_locations(const vector<map<Fraction, Group>> &locations);

    Voice location_to_notation(map<Fraction, Group> &locations);
};
