#pragma once

#include "../location/Location.h"
#include "../notation/Notation.h"
#include "../notation/NotationUtils.h"
#include "../reader/Part.h"
#include "../Translations.h"

#include <map>
#include <json/json.h>

using namespace std;

// todo: Maybe the variations need to also support elapsing.

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
    namespace Scale {
        void apply(Part &part, const Json::Value &arguments);
    }
    namespace Filter {
        void apply(Part &part, const Json::Value &arguments);
    }

    static const map<string, void (*)(Part &, const Json::Value &)> name_to_variation = \
                                                     {{"Change Note",      ChangeNote::apply},
                                                      {"Tuplet",           Tuplet::apply},
                                                      {"Double",           Double::apply},
                                                      {"Fill",             Fill::apply},
                                                      {"Sticking",         Sticking::apply},
                                                      {"Stretch Sticking", StretchSticking::apply},
                                                      {"Scale",            Scale::apply},
                                                      {"Filter",           Filter::apply}};
}
