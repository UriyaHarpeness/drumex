#include "Part.h"


Part::Part(Locations locations, TimeSignature signature, const Fraction &length) : m_location(move(locations)),
                                                                                   m_signature(move(signature)),
                                                                                   m_length(length) {}

Part::Part(const string &path, int index) {
    Json::Reader reader;
    Json::Value obj;
    ifstream part_file(path);

    if (!part_file.good()) {
        throw runtime_error("Part File Does Not Exist");
    }

    reader.parse(part_file, obj);
    part_file.close();

    Log(INFO).Get() << "Loading Part: " << obj["Name"].asString() << "[" << index << "]" << endl;
    Json::Value global_definitions = obj["Global"];
    Json::Value part = obj["Parts"][index];

    // todo: document jsons structures.
    if (part["Time Signature"].isNull()) {
        part["Time Signature"] = global_definitions["Time Signature"];
    }
    if (part["Type"].isNull()) {
        part["Type"] = global_definitions["Type"];
    }
    if ((part["Type"].asString() == "Custom") && (part["Part"]["Definitions"].isNull())) {
        part["Part"]["Definitions"] = global_definitions["Part"]["Definitions"];
    }

    m_signature = {part["Time Signature"][0].asUInt(), part["Time Signature"][1].asUInt()};

    // currently supported types are only "Regular" and "Custom", maybe will need to expand.
    if (part["Type"].asString() == "Regular") {
        m_location = read_regular_voice(part["Part"]);
    } else {
        m_location = read_custom_voice(part["Part"]);
    }

    location::optimize_location(m_location);
    m_length = prev(m_location.end())->first;
}

void Part::notationize() {
    vector<Locations> location_voices = location::split_voices_locations(m_location);
    location::optimize_location(location_voices[0]);
    location::optimize_location(location_voices[1]);

    m_up = VoiceContainer(location_voices[0], m_signature, NotesUp);
    m_down = VoiceContainer(location_voices[1], m_signature, NotesDown);
}

Notation Part::json_to_note(const Json::Value &note_json) {
    vector<Modifier> modifiers;
    Instrument inst = instrument_names.at(note_json[0].asString());
    Fraction length = {note_json[1][0].asInt(), note_json[1][1].asInt()};
    if (!note_json[2].empty()) {
        for (const auto &modifier : note_json[2]) {
            modifiers.push_back(modifier_names.at(modifier.asString()));
        }
    }

    return {(inst == Unbound) ? BaseRest : BasePlay, (inst == Unbound) ? UnboundUp : inst, length, modifiers};
}

Locations Part::read_regular_voice(const Json::Value &part) {
    Voice voice;
    Group group;
    vector<Modifier> modifiers;
    for (const auto &raw_group : part) {
        for (const auto &note : raw_group) {
            group.push_back(json_to_note(note));
            modifiers.clear();
        }
        voice.push_back(move(group));
        group.clear();
    }
    return move(location::notation_to_location(voice));
}

Locations Part::read_custom_voice(const Json::Value &part) {
    // maybe make group of notes...
    map<char, Notation> definitions;
    vector<Modifier> modifiers;

    for (const string &name : part["Definitions"].getMemberNames()) {
        const Json::Value &note = part["Definitions"][name];

        definitions.insert(pair<char, Notation>(name[0], json_to_note(note)));
        modifiers.clear();
    }

    Voice voice;
    vector<Voice> voices;
    char last = '|';
    for (const Json::Value &json_voice : part["Use"]) {
        for (const char &c : json_voice.asString()) {
            // For readability purpose.
            last = c;
            if (c == '|') {
                continue;
            }
            if (definitions.find(c) == definitions.end()) {
                throw runtime_error("Character '" + string({c}) + "' was not defined before use");
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

    return voices_to_location(voices);
}

int Part::get_parts_number(const string &path) {
    Json::Reader reader;
    Json::Value obj;
    ifstream part_file(path);

    if (!part_file.good()) {
        throw runtime_error("Part File Does Not Exist");
    }

    reader.parse(part_file, obj);
    part_file.close();

    return obj["Parts"].size();
}

Locations Part::voices_to_location(const vector<Voice> &notations) {
    vector<Locations> locations;

    for (const auto &notation : notations) {
        locations.push_back(location::notation_to_location(notation));
    }

    return move(location::merge_locations(locations));
}

void Part::multiple_length(int multiply) {
    multiple_length(m_length * Fraction(multiply));
}

void Part::multiple_length(const Fraction &new_length) {
    location::stretch_locations(m_location, new_length);
    m_length = new_length;
}
