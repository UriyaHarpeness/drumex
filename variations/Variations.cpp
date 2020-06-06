#include "Variations.h"

bool variations::match(const Notation &note, const Json::Value &instruments, const Json::Value &modifiers) {
    bool all_instruments = instruments.isNull() or instruments.empty();
    bool all_modifiers = modifiers.isNull() or modifiers.empty();

    if (all_instruments && all_modifiers) {
        return true;
    }

    if (!all_instruments && !all_modifiers) {
        for (const auto &instrument : instruments) {
            if (note.get_instrument() == instrument_names.at(instrument.asString())) {
                for (const auto &modifier : modifiers) {
                    const auto &note_modifiers = note.get_modifiers();
                    if (find(note_modifiers.begin(), note_modifiers.end(),
                             modifier_names.at(modifier.asString())) != note_modifiers.end()) {
                        return true;
                    }
                }
            }
        }
    } else if (all_instruments) {
        for (const auto &modifier : modifiers) {
            const auto &note_modifiers = note.get_modifiers();
            if (find(note_modifiers.begin(), note_modifiers.end(),
                     modifier_names.at(modifier.asString())) != note_modifiers.end()) {
                return true;
            }
        }
    } else {
        for (const auto &instrument : instruments) {
            if (note.get_instrument() == instrument_names.at(instrument.asString())) {
                return true;
            }
        }
    }

    return false;
}

Locations variations::match(const Locations &locations, const Json::Value &instruments, const Json::Value &modifiers) {
    Locations matching_locations;

    for (const auto &location : locations) {
        Group matching_group;
        for (const auto &note : location.second) {
            if (match(note, instruments, modifiers)) {
                matching_group.push_back(note);
            }
        }
        if (!matching_group.empty()) {
            matching_locations[location.first] = move(matching_group);
            matching_group.clear();
        }
    }
    matching_locations.insert(*prev(locations.end()));

    return move(matching_locations);
}

void variations::ChangeNote::apply(Part &part, const Json::Value &arguments) {
    bool override_instrument = !arguments["Apply"]["Instrument"].isNull();
    Instrument destination_instrument = (override_instrument ? instrument_names.at(
            arguments["Apply"]["Instrument"].asString()) : Unbound);
    bool override_modifiers = !arguments["Apply"]["Modifiers"].isNull();
    vector<Modifier> modifiers;

    if (override_modifiers) {
        for (const auto &modifier : arguments["Apply"]["Modifiers"]) {
            modifiers.push_back(modifier_names.at(modifier.asString()));
        }
    }

    Group new_group;
    Locations new_locations;

    for (const auto &location : part.get_location()) {
        Fraction global_offset = location.first;
        Group group = location.second;
        new_locations[global_offset] = group;

        // todo: support change only single note in group.
        for (const auto &note : group) {
            if ((note.get_playing() == BasePlay) &&
                match(note, arguments["Match"]["Instruments"], arguments["Match"]["Modifiers"])) {
                new_group.push_back({BasePlay, (override_instrument ? destination_instrument : note.get_instrument()),
                                     note.get_length(), (override_modifiers ? modifiers : note.get_modifiers())});
            }
        }
        if (!new_group.empty()) {
            new_locations[location.first] = move(new_group);
            new_group.clear();
        }
    }

    part.set_location(new_locations);
}

void variations::Tuplet::apply(Part &part, const Json::Value &arguments) {
    Locations new_locations;

    for (const auto &location : part.get_location()) {
        if (static_cast<bool>(location.first % Fraction(1, 16))) {
            throw runtime_error("Invalid Note Offset For Tuplet Variation");
        }
        Fraction offset_in_quarter = location.first % Fraction(1, 4);
        Fraction quarter_location = location.first - offset_in_quarter;
        Fraction tuplet_offset = {sixteenth_tuplet[(int) static_cast<double>(offset_in_quarter / Fraction(1, 16))],
                                  4 * 6};
        new_locations[quarter_location + tuplet_offset] = location.second;
    }

    part.set_location(new_locations);
}

void variations::Double::apply(Part &part, const Json::Value &arguments) {
    Locations new_locations;

    Fraction distance = {arguments["Distance"][0].asInt(), arguments["Distance"][1].asInt()};
    Locations matching = match(part.get_location(), arguments["Match"]["Instruments"], arguments["Match"]["Modifiers"]);
    bool carry = arguments["Carry"].asBool();

    // Treats all matching notes as the same one for that matter.
    if (!carry) {
        for (const auto &location : matching) {
            if ((matching.find(location.first + distance) == matching.end()) &&
                (location.first + distance < prev(matching.end())->first)) {
                new_locations[location.first + distance] = location.second;
            }
        }
        part.set_location(location::merge_locations({new_locations, part.get_location()}));
        return;
    }

    int play_count = 0;
    for (auto location_it = matching.begin(); location_it != prev(matching.end()); location_it++) {
        Fraction space = next(location_it)->first - location_it->first;
        play_count++;

        if (space == distance) {
            /*
             * Carry.
             * todo:
             * multiplying can have several types, for example for:
             * 0----00---0-0---
             * it can be:
             * 0o---00oo-0o0o--
             * 0-o--00oo-0-0-o-
             * 0-o--00-oo0-0-o-
             * need to see which to choose, or how to support multiple types,
             * currently it will be supported naively, assuming there won't be smaller space that the distance, this may not be always true.
             */
        } else {
            if (space < distance) {
                throw runtime_error("Double assumes that there's no space smaller than the distance");
            }

            play_count = min(play_count, (int) static_cast<double>(space / distance));
            do {
                new_locations[location_it->first + (Fraction(play_count) * distance)] = location_it->second;
            } while (--play_count);
        }
    }

    part.set_location(location::merge_locations({new_locations, part.get_location()}));
}
