#include "Part.h"


Part::Part(Notations notation, TimeSignature signature, Fraction length) : m_notation(move(notation)),
                                                                           m_signature(signature), m_length(length) {}

Part::Part(const string &path, int index) {
    Json::Reader reader;
    Json::Value obj;
    ifstream f(path);
    reader.parse(f, obj);
    f.close();

    cout << "Loading part: " << obj["Name"].asString() << "[" << index << "]" << endl;
    Json::Value part = obj["Parts"][index];

    m_signature = {part["Time Signature"][0].asUInt(), obj["Parts"][0]["Time Signature"][1].asUInt()};

    // currently supported types are only "Regular" and "Custom", maybe will need to expand.
    Voice voice;
    if (part["Type"].asString() == "Regular") {
        voice = read_regular_voice(part["Part"]);
    } else if (part["Type"].asString() == "Custom") {
        voice = read_custom_voice(part["Part"]);
    } else {
        // todo: add more understandable exceptions throughout the code.
        throw runtime_error("Unknown Part Type");
    }

    // todo: maybe generate only partially? splitting the notes correctly will need to happen again, and keeping it in
    // one voice may be easier to modify, but first regenerate the correct spacing of stuff.
    // this needs to happen on the Exercise part since he modifies the pure notation.
    m_notation = Notation::generate_notation(voice, m_signature);

    m_length = Notation::sum_final_length(m_notation);

    /*Json::Value &links = obj["parse"]["links"];

    auto *str_links = new string[links.end() - links.begin()];*/
}

Voice Part::read_regular_voice(const Json::Value &part) {
    Voice voice;
    Group group;
    vector<Modifier> modifiers;
    for (const auto &raw_group : part) {
        for (const auto &note : raw_group) {
            Instrument inst = instrument_names.at(note[0].asString());
            Fraction length = {note[1][0].asInt(), note[1][1].asInt()};
            for (const auto &modifier : note[2]) {
                modifiers.push_back(modifier_names.at(modifier.asString()));
            }
            group.push_back(
                    {(inst == Unbound) ? BaseRest : BasePlay, (inst == Unbound) ? UnboundUp : inst, length,
                     move(modifiers)});
            modifiers.clear();
        }
        voice.push_back(move(group));
        group.clear();
    }
    return move(voice);
}

Voice Part::read_custom_voice(const Json::Value &part) {
    // maybe make group of notes...
    map<char, Notation> definitions;
    vector<Modifier> modifiers;

    for (const string &name : part["Definitions"].getMemberNames()) {
        const Json::Value &note = part["Definitions"][name];
        Instrument inst = instrument_names.at(note[0].asString());
        Fraction length = {note[1][0].asInt(), note[1][1].asInt()};
        for (const auto &modifier : note[2]) {
            modifiers.push_back(modifier_names.at(modifier.asString()));
        }

        definitions.insert(pair<char, Notation>(name[0], {(inst == Unbound) ? BaseRest : BasePlay,
                                                          (inst == Unbound) ? UnboundUp : inst, length,
                                                          move(modifiers)}));
        modifiers.clear();
    }

    Voice voice;
    Notations voices;
    char last = '|';
    for (const Json::Value &json_voice : part["Use"]) {
        for (const char &c : json_voice.asString()) {
            // For readability purpose.
            last = c;
            if (c == '|') {
                continue;
            }
            voice.push_back({definitions.at(c)});
        }
        // Enables splitting long notation lines.
        if (last == '|') {
            continue;
        }

        voices.push_back(move(voice));
        voice.clear();
    }

    return move(merge_voices(voices));
}

int Part::get_parts_number(const string &path) {
    Json::Reader reader;
    Json::Value obj;
    ifstream f(path);
    reader.parse(f, obj);
    f.close();

    return obj["Parts"].size();
}

Voice Part::merge_voices(const Notations &notations) {
    vector<map<Fraction, Group>> locations;

    for (const auto &notation : notations) {
        locations.push_back(location::notation_to_location(notation));
    }

    map<Fraction, Group> merged_location = location::merge_locations(locations);

    return move(location::location_to_notation(merged_location));
}

Part Part::merge_parts(vector<Part> parts) {
    Notations notation;
    Voice voice;
    GroupedNotations notations;
    vector<Fraction> lengths;
    Fraction length;

    for (const auto &part : parts) {
        lengths.push_back(part.get_length());
    }

    Fraction merged_length = Notation::merge_lengths(lengths);

    for (auto &part : parts) {
        Notation::stretch_notation(part.get_mutable_notation(), part.get_length(), merged_length);
        notations.push_back(part.get_notation());
    }

    Notations voices;

    for (const auto &separate_notation : notations) {
        voices.push_back(separate_notation[0]);
        voices.push_back(separate_notation[1]);
    }

    voice = merge_voices(voices);

    notation = Notation::generate_notation(voice, parts[0].get_signature());

    // merged_signature denominator can't be 1, makes problem with beams and the beat, so currently solve with sort of a patch.
    // todo: solve this in some better way.
    return move(Part(notation, parts[0].get_signature(), length));
}
