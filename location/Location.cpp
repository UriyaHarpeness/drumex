#include "Location.h"

Locations location::notation_to_location(const Voice &voice) {
    Locations locations;

    Fraction notes_offset;
    for (const auto &group: voice) {
        // todo: need to think if may be possible that there will be a rest and a note in the same group.
        if (group[0].get_playing() == BasePlay) {
            locations[notes_offset] = group;
        } else {
            if (locations.empty()) {
                assert(!static_cast<bool>(notes_offset));
                locations[notes_offset] = group;
            }
            auto last_location = locations.end();
            last_location--;
            for (auto &note: last_location->second) {
                note.reset_length(note.get_length() + group[0].get_length());
            }
        }
        notes_offset += group[0].get_length();
    }
    locations[notes_offset] = {};

    return move(locations);
}

void location::clear_location(Locations &locations) {
    // Without removing the last location.
    for (auto it = locations.begin(); it != prev(locations.end());) {
        it->second.erase(remove_if(it->second.begin(), it->second.end(),
                                   [](const Notation &note) { return note.get_playing() == BaseRest; }),
                         it->second.end());
        if (it->second.empty()) {
            it = locations.erase(it);
        } else {
            it++;
        }
    }
}

void location::optimize_location(Locations &locations) {
    location::clear_location(locations);

    for (auto it = locations.begin(); it != prev(locations.end()); it++) {
        for (auto &note: it->second) {
            note.reset_length(next(it)->first - it->first);
        }
    }
}

Locations location::merge_locations(const vector<Locations> &locations) {
    Locations merged_locations;
    Group group;

    // The returned locations are clean of rests.
    for (const auto &single_location: locations) {
        for (const auto &location: single_location) {
            for (const auto &note: location.second) {
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

    // The lengths of the different locations need to be the same.
    merged_locations.insert((*prev(locations[0].end())));

    return merged_locations;
}

Locations location::fit_locations(const Locations &locations, const Fraction &final_length) {
    Locations fitted_locations;
    Fraction length = prev(locations.end())->first;
    auto location_it = locations.begin();
    Fraction offset;

    while (offset + location_it->first < final_length) {
        fitted_locations[offset + location_it->first] = location_it->second;

        location_it++;
        if (location_it == prev(locations.end())) {
            offset += length;
            location_it = locations.begin();
        }
    }
    fitted_locations[final_length] = {};

    return move(fitted_locations);
}

vector<Locations> location::split_voices_locations(const Locations &locations) {
    vector<Locations> split_locations = {{},
                                         {}};
    Group up;
    Group down;
    for (const auto &location: locations) {
        for (const auto &note: location.second) {
            if (DisplayConstants::direction_line >= note.get_line()) {
                up.push_back(note);
            } else {
                down.push_back(note);
            }
        }
        if (!up.empty()) {
            split_locations[0].insert({location.first, move(up)});
            up.clear();
        }
        if (!down.empty()) {
            split_locations[1].insert({location.first, move(down)});
            down.clear();
        }
    }
    split_locations[0].insert({prev(locations.end())->first, {}});
    split_locations[1].insert({prev(locations.end())->first, {}});
    return move(split_locations);
}

Voice location::location_to_notation(Locations &locations, Instrument rests_location, const Fraction &ratio) {
    Voice voice;
    Group group;

    auto location = locations.begin();
    auto prev_note = locations.begin();
    if (location->first) {
        voice.push_back({{BaseRest, rests_location, location->first * ratio, {}}});
    }
    for (; location != prev(locations.end()); location++) {
        if (location->second.empty()) {
            for (auto &note: voice[voice.size() - 1]) {
                // length should be without ModDots at this point.
                note.reset_length((next(location)->first - prev_note->first) * ratio);
            }
            continue;
        }

        group = location->second;
        prev_note = location;

        for (auto &note: group) {
            note.reset_length((next(location)->first - location->first) * ratio);
        }

        voice.push_back(move(group));
        group.clear();
    }

    return move(voice);
}
