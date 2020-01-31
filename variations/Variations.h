#pragma once

#include "../notation/Notation.h"

#include <map>
#include <jsoncpp/json/json.h>

using namespace std;

/*  needs to be here
    void convert_time_4_to_6(uint32_t note_value);

    void convert_time_multiply(uint32_t note_value);

    void convert_time_stretch(uint32_t note_value);

    void convert_time(uint32_t note_value);
 */

namespace variations {
    namespace DoubleNotes {
        void apply(Notations &notation, Json::Value arguments);
    }
    namespace QuickDoubleGhosts {
        void apply(Notations &notation, Json::Value arguments);
    }
    namespace PlayRight {
        void apply(Notations &notation, Json::Value arguments);
    }
    namespace PlayLeft {
        void apply(Notations &notation, Json::Value arguments);
    }
    namespace StretchTimeSignature {
        void apply(Notations &notation, Json::Value arguments);
    }
    namespace ExtendTimeSignature {
        void apply(Notations &notation, Json::Value arguments);
    }
    const map<string, void (*)(Notations &, Json::Value)> name_to_variation = \
                                                     {{"Double Notes",           DoubleNotes::apply},
                                                      {"Quick Double Ghosts",    QuickDoubleGhosts::apply},
                                                      {"Play Right",             PlayRight::apply},
                                                      {"Play Left",              PlayLeft::apply},
                                                      {"Stretch Time Signature", StretchTimeSignature::apply},
                                                      {"Extend Time Signature",  ExtendTimeSignature::apply}};
}
