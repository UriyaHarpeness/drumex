#include "Part.h"

const map<string, Instrument> Part::instrument_names = \
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

const map<string, Modifier> Part::modifier_names = \
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


Part::Part(const string &path, int index) {
    Json::Reader reader;
    Json::Value obj;
    ifstream f(path);
    reader.parse(f, obj);

    cout << "Loading part: " << obj["Name"].asString() << "[" << index << "]" << endl;
    Json::Value part = obj["Parts"][index];

    m_signature = {part["Time Signature"][0].asUInt(), obj["Parts"][0]["Time Signature"][1].asUInt()};

    Voice voice;
    Group group;
    vector<Modifier> modifiers;
    for (const auto &raw_group : part["Part"]) {
        for (const auto &note : raw_group) {
            Instrument inst = instrument_names.at(note[0].asString());
            Fraction length = {note[1][0].asInt(), note[1][1].asInt()};
            for (const auto &modifier : note[2]) {
                modifiers.push_back(modifier_names.at(modifier.asString()));
            }
            group.push_back({(inst == Unbound) ? BaseRest : BasePlay, inst, length, modifiers});
            modifiers.clear();
        }
        voice.push_back(group);
        group.clear();
    }

    m_notation = Notation::generate_notation(voice, m_signature);

    /*Json::Value &links = obj["parse"]["links"];

    auto *str_links = new string[links.end() - links.begin()];*/
}
