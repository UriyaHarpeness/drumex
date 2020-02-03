#pragma once

#include "../notation/Notation.h"

namespace location {
    map<Fraction, vector<Notation>> create_location_mapping(const vector<vector<Notation>> &voice);

    Voice location_to_notation(map<Fraction, Group> &locations);
};
