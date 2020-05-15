#pragma once

#include "../notation/Notation.h"
#include "../notation/NotationUtils.h"
#include "../notation_container/VoiceContainer.h"
#include "../Translations.h"
#include "../location/Location.h"

#include <fstream>
#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>
#include <utility>


using namespace std;

class Part {
public:
    Part() = default;

    Part(Locations locations, TimeSignature signature, const Fraction &length);

    Part(const string &path, int index);

    void notationize();

    static Notation json_to_note(const Json::Value &note_json);

    static Locations read_regular_voice(const Json::Value &part);

    static Locations read_custom_voice(const Json::Value &part);

    static int get_parts_number(const string &path);

    static Locations voices_to_location(const Notations &notations);

    static Part merge_parts(vector<Part> parts);

    [[nodiscard]] inline VoiceContainer &get_up() { return m_up; };

    [[nodiscard]] inline VoiceContainer &get_down() { return m_down; };

    [[nodiscard]] inline Notations &get_mutable_notation() { return m_notation; };

    inline void set_location(const Locations &location) { m_location = location; };

    [[nodiscard]] inline const Locations &get_location() const { return m_location; };

    [[nodiscard]] inline Locations &get_mutable_location() { return m_location; };

    [[nodiscard]] inline TimeSignature get_signature() const { return m_signature; };

    [[nodiscard]] inline Fraction get_length() const { return m_length; };

private:
    TimeSignature m_signature;

    Notations m_notation;

    VoiceContainer m_up;

    VoiceContainer m_down;

    Locations m_location;

    Fraction m_length;
};
