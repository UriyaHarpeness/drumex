#pragma once

#include "../location/Location.h"
#include "../notation/Notation.h"
#include "../notation/NotationUtils.h"
#include "../reader/Part.h"
#include "../Translations.h"

#include <map>
#include <jsoncpp/json/json.h>

using namespace std;

/**
 * todo: List of variations to implement:
 *
 * 1. + Change note.
 * 2. + Convert 4/4 to triplets.
 * 3. + Double notes - considering spacing, carrying, and breaking.
 * 4. + Fill notes with fixed spacing.
 * 5. + Right left alternation.
 * 6. + Stretch sticking (continue current R/L).
 * 7.   3 bars of random beat and than one of exercise, chester for example - may need to make this an exercise setting.
 * 8.   Stretch time - for example 1/16 to 1/32.
 * 9.   Filter notes, plus inverted to delete notes.
 * 10.  Change signature maybe.
 *
 * The variations need to also support elapsing.
 */

namespace variations {
    bool match(const Notation &note, const Json::Value &instruments, const Json::Value &modifiers);

    Locations match(const Locations &locations, const Json::Value &instruments, const Json::Value &modifiers);

    namespace ChangeNote {
        void apply(Part &part, const Json::Value &arguments);
    }
    namespace Tuplet {
        static const array<int, 4> sixteenth_tuplet = {0,
                                                       2,
                                                       3,
                                                       5};

        void apply(Part &part, const Json::Value &arguments);
    }
    namespace Double {
        void apply(Part &part, const Json::Value &arguments);
    }
    namespace Fill {
        void apply(Part &part, const Json::Value &arguments);
    }
    namespace Sticking {
        void apply(Part &part, const Json::Value &arguments);
    }
    namespace StretchSticking {
        void apply(Part &part, const Json::Value &arguments);
    }

    static const map<string, void (*)(Part &, const Json::Value &)> name_to_variation = \
                                                     {{"Change Note",      ChangeNote::apply},
                                                      {"Tuplet",           Tuplet::apply},
                                                      {"Double",           Double::apply},
                                                      {"Fill",             Fill::apply},
                                                      {"Sticking",         Sticking::apply},
                                                      {"Stretch Sticking", StretchSticking::apply}};
}
