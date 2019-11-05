#pragma once

#include "../part/Part.h"
#include "../Enums.h"
#include "../variations/Variations.h"

#include <string>
#include <iostream>
#include <map>

using namespace std;

class Role {
public:
    static const map<char, Playing> char_to_playing;

    static const map<char, Sticking> char_to_sticking;

    Role(const Role &other);

    Role(Instrument instrument, const string &part, TimeSignature time_signature);

    Role(Instrument instrument, const string &part, const string &sticking, TimeSignature time_signature);

    pair<Instrument, Action> play();

    static Actions generate_actions(const string &part);

    static Actions generate_actions(const string &part, const string &sticking);

    [[nodiscard]]
    const Instrument &get_instrument() const;

    void set_instrument(const Instrument &instrument);

    Part &get_part();

    void apply_variations(const Json::Value &variations);

protected:
    Instrument m_instrument;

    Part m_part;

    /**
     * A time signature has the form of x/y, while x is the number of beats in each measure (bar), and y is the note
     * value equivalent to a beat.
     * So 3/4 means that there are 3 quarter notes in a bar.
     */
};
