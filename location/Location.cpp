#include "Location.h"

map<Fraction, Group> location::create_location_mapping(const Voice &voice) {
    map<Fraction, Group> locations;

    // todo: maybe simply use 0,1 for default constructor and be over with all this initialization stuff.
    // todo: think what to do with notes with value below minimum supported fraction on different lines.
    Fraction notes_offset;
    for (const auto &group : voice) {
        locations[notes_offset] = group;
        notes_offset += group[0].get_length();
    }
    locations[notes_offset] = {};

    return move(locations);
}

Voice location::location_to_notation(map<Fraction, Group> &locations) {
    Voice voice;
    Group group;

    // todo: think what to do with notes with value below minimum supported fraction on different lines.
    // add dots to get to the exact needed length.
    Fraction notes_offset;
    for (auto location = locations.begin(); location != locations.end(); location++) {
        if (location->second.empty()) {
            continue;
        }
        group = location->second;
        // todo: it's more than adding dots here and there, it can change the whole notation structure with new rests...

        for (auto &note : group) {
            note.set_rounded_length(next(location)->first - location->first);
        }
        voice.push_back(group);
        group.clear();
    }

    return move(voice);
}
