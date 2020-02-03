#include "Variations.h"


bool variations::match(const Notation &note, const Json::Value &instruments, const Json::Value &modifiers) {
    // todo: treat empty as all acceptable.
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
    return false;
}

void variations::DoubleNotes::apply(Notations &notation, Json::Value arguments) {
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

void variations::QuickDouble::apply(Notations &notation, Json::Value arguments) {
    cout << "Applying variation: QuickDouble" << endl;

    // overriding is not possible.

    // todo: maybe keep the notation in a map of global locations, at least at first for easy distance management and changes.

    Fraction distance = {arguments["Distance"][0].asInt(), arguments["Distance"][1].asInt()};

    // todo: better for loop.
    // todo: support identifying be modifiers, maybe extract to function.
    Group new_group;
    for (int i = 0; i < notation.size(); i++) {
        auto &voice = notation[i];
        auto locations = location::create_location_mapping(voice);
        map<Fraction, Group> new_locations;

        for (auto &location : locations) {
            Fraction global_offset = location.first;
            Group group = location.second;
            new_locations[global_offset] = group;

            for (auto &note : group) {
                if (match(note, arguments["Instruments"], arguments["Modifiers"])) {
                    // easy case, otherwise overriding needs to be checked, plus other difficult logic, lets go!
                    // can be problematic with modifiers, mostly isn't the case, currently let it slide.

                    // todo: use only the global mapping, not the notes distance, and later adjust the notes distance
                    // to the mapping.
                    // also, think about the overlapping, and rounding the trespassing notes to the beginning.
                    if (note.get_length() > distance) {
                        cout << "apply here" << endl;
                        new_group.push_back({BasePlay, SnareInst, note.get_length() - distance, note.get_modifiers()});
                    }
                }
            }
            if (!new_group.empty()) {
                new_locations[location.first + distance] = new_group;
                new_group.clear();
            }
        }
        notation[i] = location::location_to_notation(new_locations);
    }
    /*
    TimeSignature time_signature = part.get_time_signature();
    time_signature.first *= 2;
    time_signature.second *= 2;
    Actions actions(time_signature.first);
    size_t index = 0;
    for (const Action &action : part.get_actions()) {
        if (get<0>(action) == Ghost) {
            actions[index++] = make_tuple(Ghost, get<1>(action));
            actions[index++] = make_tuple(Ghost, get<1>(action));
        } else {
            actions[index++] = action;
            actions[index++] = make_tuple(Rest, UnboundStick);
        }
    }
    part.set_actions(actions);
    part.set_time_signature(time_signature);
    */
}

void variations::PlayRight::apply(Notations &notation, Json::Value arguments) {
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

void variations::PlayLeft::apply(Notations &notation, Json::Value arguments) {
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

void variations::StretchTimeSignature::apply(Notations &notation, Json::Value arguments) {
    /*
    printf("variations::StretchTimeSignature::apply\n");

    // todo: this currently multiplies, need to support some time in "as triplets"
    part.convert_time_stretch(arguments[1].asInt());
    */
}

void variations::ExtendTimeSignature::apply(Notations &notation, Json::Value arguments) {
    /*
    // todo: only words in whole note results, like 3/3 or 4/4
    TimeSignature time_signature = part.get_time_signature();
    time_signature.second = arguments[1].asInt();
    part.set_time_signature(time_signature);
    */
}
