#include "Part.h"


Part::Part(Notations notation, TimeSignature signature) : m_notation(notation), m_signature(signature) {}

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

Part Part::merge_parts(vector<Part> parts) {
    Notations notation;
    Voice voice;
    vector<Notations> notations;
    vector<TimeSignature> signatures;

    for (const auto &part : parts) {
        signatures.push_back(part.get_signature());
    }

    TimeSignature merged_signature = Notation::merge_time_signatures(signatures);

    for (auto &part : parts) {
        Notation::stretch_notation(part.get_mutable_notation(), part.get_signature(), merged_signature);
        notations.push_back(part.get_notation());
    }

    Notations merged_full_notation;
    vector<map<Fraction, Group>> locations;

    for (const auto &separate_notation : notations) {
        locations.push_back(location::create_location_mapping(separate_notation[0]));
        locations.push_back(location::create_location_mapping(separate_notation[1]));
    }

    // todo: may need to calc the time signature before and pass it down.
    // after that stretch the notes to have the same length with the calculated signature.
    map<Fraction, Group> merged_location = location::merge_locations(locations);

    voice = location::location_to_notation(merged_location);

    notation = Notation::generate_notation(voice, signatures[0]);

    // merged_signature denominator can't be 1, makes problem with beams and the beat, so currently solve with sort of a patch.
    // todo: solve this in some better way.
    return Part(notation, signatures[0]);
}
