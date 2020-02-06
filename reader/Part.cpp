#include "Part.h"


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
            group.push_back(
                    {(inst == Unbound) ? BaseRest : BasePlay, (inst == Unbound) ? UnboundUp : inst, length, modifiers});
            modifiers.clear();
        }
        voice.push_back(group);
        group.clear();
    }

    // todo: maybe generate only partially? splitting the notes correctly will need to happen again, and keeping it in
    // one voice may be easier to modify, but first regenerate the correct spacing of stuff.
    // this needs to happen on the Exercise part since he modifies the pure notation.
    m_notation = Notation::generate_notation(voice, m_signature);

    /*Json::Value &links = obj["parse"]["links"];

    auto *str_links = new string[links.end() - links.begin()];*/
}

int Part::get_parts_number(const string &path) {
    Json::Reader reader;
    Json::Value obj;
    ifstream f(path);
    reader.parse(f, obj);

    return obj["Parts"].size();
}
