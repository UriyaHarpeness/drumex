#pragma once

#include "../notation/Notation.h"
#include "../notation/NotationUtils.h"
#include "../Translations.h"
#include "../location/Location.h"

#include <fstream>
#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>


using namespace std;

class Part {
public:
    Part() = default;

    Part(Notations notation, const TimeSignature &signature, const Fraction &length);

    Part(const string &path, int index);

    static Voice read_regular_voice(const Json::Value &part);

    static Voice read_custom_voice(const Json::Value &part);

    static int get_parts_number(const string &path);

    static Voice merge_voices(const vector<Voice> &voices);

    static Part merge_parts(vector<Part> parts);

    [[nodiscard]] inline const Notations &get_notation() const { return m_notation; };

    [[nodiscard]] inline Notations &get_mutable_notation() { return m_notation; };

    [[nodiscard]] inline TimeSignature get_signature() const { return m_signature; };

    [[nodiscard]] inline Fraction get_length() const { return m_length; };

private:
    TimeSignature m_signature;

    Notations m_notation;

    Fraction m_length;
};
