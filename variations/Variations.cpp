#include "Variations.h"

void variations::DoubleNotes::apply(Part &part, Json::Value arguments) {
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
}

void variations::QuickDoubleGhosts::apply(Part &part, Json::Value arguments) {
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
}

void variations::PlayRight::apply(Part &part, Json::Value arguments) {
    printf("variations::PlayRight::apply\n");

    TimeSignature time_signature = part.get_time_signature();
    Actions actions(time_signature.first);
    size_t index = 0;
    for (const Action &action : part.get_actions()) {
        actions[index++] = make_tuple(get<0>(action), (get<0>(action) == Rest) ? UnboundStick : Right);
    }
    part.set_actions(actions);
    part.set_time_signature(time_signature);
}

void variations::PlayLeft::apply(Part &part, Json::Value arguments) {
    TimeSignature time_signature = part.get_time_signature();
    Actions actions(time_signature.first);
    size_t index = 0;
    for (const Action &action : part.get_actions()) {
        actions[index++] = make_tuple(get<0>(action), (get<0>(action) == Rest) ? UnboundStick : Left);
    }
    part.set_actions(actions);
    part.set_time_signature(time_signature);
}

void variations::StretchTimeSignature::apply(Part &part, Json::Value arguments) {
    printf("variations::StretchTimeSignature::apply\n");

    // todo: this currently multiplies, need to support some time in "as triplets"
    part.convert_time_stretch(arguments[1].asInt());
}

void variations::ExtendTimeSignature::apply(Part &part, Json::Value arguments) {
    // todo: only words in whole note results, like 3/3 or 4/4
    TimeSignature time_signature = part.get_time_signature();
    time_signature.second = arguments[1].asInt();
    part.set_time_signature(time_signature);
}
