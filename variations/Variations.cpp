#include "Variations.h"


bool variations::match(const Notation &note, const Json::Value &instruments, const Json::Value &modifiers) {
    bool all_instruments = instruments.empty();
    bool all_modifiers = modifiers.empty();

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
    Group new_group;
    for (auto &voice : notation) {
        auto locations = location::notation_to_location(voice);
        map<Fraction, Group> new_locations;

        for (const auto &location : locations) {
            Fraction global_offset = location.first;
            Group group = location.second;
            new_locations[global_offset] = group;

            for (const auto &note : group) {
                if ((note.get_playing() == BasePlay) && match(note, arguments["Instruments"], arguments["Modifiers"])) {
                    // easy case, otherwise overriding needs to be checked, plus other difficult logic, lets go!
                    // can be problematic with modifiers, mostly isn't the case, currently let it slide.

                    // todo: use only the global mapping, not the notes distance, and later adjust the notes distance
                    // to the mapping.
                    // also, think about the overlapping, and rounding the trespassing notes to the beginning.
                    if (note.get_length() > distance) {
                        new_group.push_back(
                                {BasePlay, note.get_instrument(), note.get_length() - distance, note.get_modifiers()});
                    }
                }
            }
            if (!new_group.empty()) {
                new_locations[location.first + distance] = new_group;
                new_group.clear();
            }
        }
        voice = Notation::generate_voice_notation(location::location_to_notation(new_locations), part.get_signature());
    }
}

void variations::ChangeNote::apply(Part &part, const Json::Value &arguments) {
    Notations &notation = part.get_mutable_notation();

    // overriding is not possible.

    // todo: maybe keep the notation in a map of global locations, at least at first for easy distance management and changes.

    Instrument destination_instrument = instrument_names.at(arguments["DestinationInstrument"].asString());

    // todo: better for loop.
    Group new_group;
    for (auto &voice : notation) {
        auto locations = location::notation_to_location(voice);
        map<Fraction, Group> new_locations;

        for (const auto &location : locations) {
            Fraction global_offset = location.first;
            Group group = location.second;
            new_locations[global_offset] = group;

            for (const auto &note : group) {
                if ((note.get_playing() == BasePlay) && match(note, arguments["Instruments"], arguments["Modifiers"])) {
                    // easy case, otherwise overriding needs to be checked, plus other difficult logic, lets go!
                    // can be problematic with modifiers, mostly isn't the case, currently let it slide.

                    // todo: use only the global mapping, not the notes distance, and later adjust the notes distance
                    // to the mapping.
                    // also, think about the overlapping, and rounding the trespassing notes to the beginning.
                    new_group.push_back(
                            {BasePlay, destination_instrument, note.get_length(), note.get_modifiers()});
                }
            }
            if (!new_group.empty()) {
                new_locations[location.first] = move(new_group);
                new_group.clear();
            }
        }
        voice = Notation::generate_voice_notation(location::location_to_notation(new_locations), part.get_signature());
    }
}

void variations::PlayRight::apply(Part &part, const Json::Value &arguments) {
    /*printf("variations::PlayRight::apply\n");

    TimeSignature time_signature = part.get_time_signature();
    Actions actions(time_signature.first);
    size_t index = 0;
    for (const Action &action : part.get_actions()) {
        actions[index++] = make_tuple(get<0>(action), (get<0>(action) == Rest) ? UnboundStick : Right);
    }
    part.set_actions(actions);
    part.set_time_signature(time_signature);
    */
}

void variations::PlayLeft::apply(Part &part, const Json::Value &arguments) {
    /*
    TimeSignature time_signature = part.get_time_signature();
    Actions actions(time_signature.first);
    size_t index = 0;
    for (const Action &action : part.get_actions()) {
        actions[index++] = make_tuple(get<0>(action), (get<0>(action) == Rest) ? UnboundStick : Left);
    }
    part.set_actions(actions);
    part.set_time_signature(time_signature);
    */
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
