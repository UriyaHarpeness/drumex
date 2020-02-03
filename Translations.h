#pragma once

#include "Enums.h"

#include <map>
#include <string>

using namespace std;

const map<string, Instrument> instrument_names = \
        {{"Rest",     Unbound},
         {"China",    ChinaInst},
         {"Crash",    CrashInst},
         {"HiHat",    HiHatInst},
         {"HighTom",  HighTomInst},
         {"MidTom",   MidTomInst},
         {"Snare",    SnareInst},
         {"LowTom",   LowTomInst},
         {"FloorTom", FloorTomInst},
         {"Bass",     BassInst},};

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
