#pragma once

#include "../part/Part.h"

#include <map>
#include <jsoncpp/json/json.h>

using namespace std;

namespace variations {
    namespace DoubleNotes {
        void apply(Part &part, Json::Value arguments);
    }
    namespace QuickDoubleGhosts {
        void apply(Part &part, Json::Value arguments);
    }
    namespace PlayRight {
        void apply(Part &part, Json::Value arguments);
    }
    namespace PlayLeft {
        void apply(Part &part, Json::Value arguments);
    }
    namespace StretchTimeSignature {
        void apply(Part &part, Json::Value arguments);
    }
    namespace ExtendTimeSignature {
        void apply(Part &part, Json::Value arguments);
    }
    const map<string, void (*)(Part &, Json::Value)> name_to_variation = \
                                                     {{"Double Notes",           DoubleNotes::apply},
                                                      {"Quick Double Ghosts",    QuickDoubleGhosts::apply},
                                                      {"Play Right",             PlayRight::apply},
                                                      {"Play Left",              PlayLeft::apply},
                                                      {"Stretch Time Signature", StretchTimeSignature::apply},
                                                      {"Extend Time Signature",  ExtendTimeSignature::apply}};
}
