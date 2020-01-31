#pragma once

#include "../notation/Notation.h"

#include <fstream>
#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>


class Part {
public:
    static const map<string, Instrument> instrument_names;

    static const map<string, Modifier> modifier_names;

    Part(const string &path, int index);

    [[nodiscard]] inline const Notations &get_notation() const { return m_notation; };

    [[nodiscard]] inline TimeSignature get_signature() const { return m_signature; };

private:
    TimeSignature m_signature;

    Notations m_notation;
};
