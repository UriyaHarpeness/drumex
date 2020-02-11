#include "Location.h"

map<Fraction, Group> location::notation_to_location(const Voice &voice) {
    map<Fraction, Group> locations;

    // todo: think what to do with notes with value below minimum supported fraction on different lines.
    Fraction notes_offset;
    for (const auto &group : voice) {
        // todo: need to think if may be possible that there will be a rest and a note in the same group.
        if (group[0].get_playing() == BasePlay) {
            locations[notes_offset] = group;
        }
        notes_offset += group[0].get_length();
    }
    locations[notes_offset] = {};

    return move(locations);
}

map<Fraction, Group> location::merge_locations(const vector<map<Fraction, Group>> &locations) {
    map<Fraction, Group> merged_locations;
    Group group;

    for (const auto &single_location : locations) {
        for (const auto &location : single_location) {
            for (const auto &note : location.second) {
                if (note.get_playing() == BasePlay) {
                    group.push_back(note);
                }
            }
            if (!group.empty()) {
                if (merged_locations.find(location.first) == merged_locations.end()) {
                    merged_locations[location.first] = location.second;
                } else {
                    merged_locations[location.first].insert(merged_locations[location.first].end(),
                                                            location.second.begin(), location.second.end());
                }
            }
        }
    }

    return merged_locations;
}

Voice location::location_to_notation(map<Fraction, Group> &locations) {
    Voice voice;
    Group group;

    // todo: think what to do with notes with value below minimum supported fraction on different lines.
    // add dots to get to the exact needed length.
    Fraction notes_offset;
    auto location = locations.begin();
    if (location->first) {
        voice.push_back({{BaseRest, UnboundUp, location->first, {}}});
    }
    auto end = locations.end();
    end--;
    for (; location != end; location++) {
        if (location->second.empty()) {
            for (auto &note : voice[voice.size() - 1]) {
                // length should be without ModDots at this point.
                note.reset_length(note.get_rounded_length() + location->first);
            }
        }
        group = location->second;

        for (auto &note : group) {
            note.reset_length(next(location)->first - location->first);
        }
        voice.push_back(move(group));
        group.clear();
    }

    return move(voice);
}
