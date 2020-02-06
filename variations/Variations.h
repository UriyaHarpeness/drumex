#pragma once

#include "../location/Location.h"
#include "../notation/Notation.h"
#include "../reader/Part.h"
#include "../Translations.h"

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
    bool match(const Notation &note, const Json::Value &instruments, const Json::Value &modifiers);

    namespace DoubleNotes {
        void apply(Part &part, const Json::Value &arguments);
    }
    namespace QuickDouble {
        void apply(Part &part, const Json::Value &arguments);
    }
    namespace PlayRight {
        void apply(Part &part, const Json::Value &arguments);
    }
    namespace PlayLeft {
        void apply(Part &part, const Json::Value &arguments);
    }
    namespace StretchTimeSignature {
        void apply(Part &part, const Json::Value &arguments);
    }
    namespace ExtendTimeSignature {
        void apply(Part &part, const Json::Value &arguments);
    }
    const map<string, void (*)(Part &, const Json::Value &)> name_to_variation = \
                                                     {{"Double Notes",           DoubleNotes::apply},
                                                      {"Quick Double",           QuickDouble::apply},
                                                      {"Play Right",             PlayRight::apply},
                                                      {"Play Left",              PlayLeft::apply},
                                                      {"Stretch Time Signature", StretchTimeSignature::apply},
                                                      {"Extend Time Signature",  ExtendTimeSignature::apply}};
}
