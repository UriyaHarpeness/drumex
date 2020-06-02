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

void variations::DoubleNotes::apply(Part &part, const Json::Value &arguments) {
    /*
    TimeSignature time_signature = part.get_time_signature();
    // the nature of doubling notes can cause overriding, but since it is a single role, which should not have left and
    // right sticking at the same time, this is ok.
    const Actions &part_actions = part.get_actions();
    Actions actions(part_actions);
    for (size_t index = 0; index < time_signature.first; index++) {
        if (get<0>(part_actions[index]) != Rest) {
            actions[(index + 1) % time_signature.first] = part_actions[index];
        }
    }
    part.set_actions(actions);
    */
}

void variations::QuickDouble::apply(Part &part, const Json::Value &arguments) {
    Notations &notation = part.get_mutable_notation();

    // overriding is not possible.
    // todo: will need to see the whole parts of rests, for example seeing 1/2 rest as 2 1/4 rests...
    // may be solvable with some conversion function, actually aplit notation may just be that.

    // todo: maybe keep the notation in a map of global locations, at least at first for easy distance management and changes.

    Fraction distance = {arguments["Distance"][0].asInt(), arguments["Distance"][1].asInt()};

    // todo: better for loop.
    int play_count = 0;
    bool triggered_play;
    for (auto &voice : notation) {
        auto locations = location::notation_to_location(voice);
        Locations new_locations;

        for (const auto &location : locations) {
            Fraction global_offset = location.first;
            Group group = location.second;
            new_locations[global_offset] = group;
            triggered_play = false;
            for (const auto &note : group) {
                if (match(note, arguments["Instruments"], arguments["Modifiers"])) {
                    // easy case, otherwise overriding needs to be checked, plus other difficult logic, lets go!
                    // can be problematic with modifiers, mostly isn't the case, currently let it slide.

                    // todo: use only the global mapping, not the notes distance, and later adjust the notes distance
                    // to the mapping.
                    // also, think about the overlapping, and rounding the trespassing notes to the beginning.
                    if (triggered_play) {
                        break;
                    }
                    triggered_play = true;
                    play_count = 1;
                    if (note.get_length() > distance) {
                        for (Fraction play = global_offset + distance;
                             (play < global_offset + note.get_length()) &&
                             (play_count > 0); play_count--, play += distance) {
                            new_locations[play] = {{BasePlay, note.get_instrument(),
                                                           note.get_length() - distance, note.get_modifiers()}};
                        }
                        play_count = 0;
                    } else if (note.get_length() == distance) {
                        // todo: what if the next note is irrelevant to the doubled notes?
                        //new_locations[location.first + distance].push_back(
                        //        {BasePlay, note.get_instrument(), note.get_length() - distance, note.get_modifiers()});
                    } else {
                        // What otherwise?
                    }
                }
            }
        }
        voice = NotationUtils::generate_voice_notation(location::location_to_notation(new_locations),
                                                       part.get_signature());
    }
}

void variations::QuickDoubleCarry::apply(Part &part, const Json::Value &arguments) {
    Notations &notation = part.get_mutable_notation();

    // overriding is not possible.
    // todo: will need to see the whole parts of rests, for example seeing 1/2 rest as 2 1/4 rests...
    // may be solvable with some conversion function, actually split notation may just be that.

    // todo: maybe keep the notation in a map of global locations, at least at first for easy distance management and changes.

    Fraction distance = {arguments["Distance"][0].asInt(), arguments["Distance"][1].asInt()};

    // todo: better for loop.
    int play_count = 0;
    bool triggered_play;
    for (auto &voice : notation) {
        auto locations = location::notation_to_location(voice);
        Locations new_locations;

        for (const auto &location : locations) {
            Fraction global_offset = location.first;
            Group group = location.second;
            new_locations[global_offset] = group;
            triggered_play = false;
            for (const auto &note : group) {
                if (match(note, arguments["Instruments"], arguments["Modifiers"])) {
                    // easy case, otherwise overriding needs to be checked, plus other difficult logic, lets go!
                    // can be problematic with modifiers, mostly isn't the case, currently let it slide.

                    // todo: use only the global mapping, not the notes distance, and later adjust the notes distance
                    // to the mapping.
                    // also, think about the overlapping, and rounding the trespassing notes to the beginning.
                    if (triggered_play) {
                        break;
                    }
                    triggered_play = true;
                    play_count++;
                    if (note.get_length() > distance) {
                        for (Fraction play = global_offset + distance;
                             (play < global_offset + note.get_length()) &&
                             (play_count > 0); play_count--, play += distance) {
                            new_locations[play] = {{BasePlay, note.get_instrument(),
                                                           note.get_length() - distance, note.get_modifiers()}};
                        }
                        play_count = 0;
                    } else if (note.get_length() == distance) {
                        // todo: what if the next note is irrelevant to the doubled notes?
                        //new_locations[location.first + distance].push_back(
                        //        {BasePlay, note.get_instrument(), note.get_length() - distance, note.get_modifiers()});
                    } else {
                        // What otherwise?
                    }
                }
            }
        }
        voice = NotationUtils::generate_voice_notation(location::location_to_notation(new_locations),
                                                       part.get_signature());
    }
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

void variations::StretchTimeSignature::apply(Part &part, const Json::Value &arguments) {
    /*
    printf("variations::StretchTimeSignature::apply\n");

    // todo: this currently multiplies, need to support some time in "as triplets"
    part.convert_time_stretch(arguments[1].asInt());
    */
}

void variations::ExtendTimeSignature::apply(Part &part, const Json::Value &arguments) {
    /*
    // todo: only words in whole note results, like 3/3 or 4/4
    TimeSignature time_signature = part.get_time_signature();
    time_signature.second = arguments[1].asInt();
    part.set_time_signature(time_signature);
    */
}
