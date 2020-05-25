#pragma once

#include "Enums.h"

#include <map>
#include <string>

using namespace std;

const map<string, BasicPlaying> playing_names = \
        {{"Play", BasePlay},
         {"Rest", BaseRest}};

const map<string, Instrument> instrument_names = \
        {{"Up",       UnboundUp},
         {"Down",     UnboundDown},
         {"Rest",     Unbound},
         {"China",    ChinaInst},
         {"Crash",    CrashInst},
         {"HiHat",    HiHatInst},
         {"HighTom",  HighTomInst},
         {"MidTom",   MidTomInst},
         {"Snare",    SnareInst},
         {"LowTom",   LowTomInst},
         {"FloorTom", FloorTomInst},
         {"Bass",     BassInst}};

const map<string, Modifier> modifier_names = \
    {{"CrossStick", ModCrossStick},
     {"Ghost",      ModGhost},
     {"Accent",     ModAccent},
     {"Dot",        ModDot},
     {"Rimshot",    ModRimshot},
     {"Flam",       ModFlam},
     {"Drag",       ModDrag},
     {"Close",      ModClose},
     {"Loose",      ModLoose},
     {"OpenClose",  ModOpenClose},
     {"Open",       ModOpen},
     {"Choke",      ModChoke},
     {"Right",      ModRight},
     {"Left",       ModLeft}};

template<typename T>
string find_by_value(const map<string, T> &where, T what) {
    for (const auto &it : where) {
        if (it.second == what) {
            return it.first;
        }
    }
    throw runtime_error("Key not Found");
}
