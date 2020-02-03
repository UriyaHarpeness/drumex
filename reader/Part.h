#pragma once

#include "../notation/Notation.h"
#include "../Translations.h"

#include <fstream>
#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>


class Part {
public:
    Part(const string &path, int index);

    static int get_parts_number(const string &path);

    [[nodiscard]] inline const Notations &get_notation() const { return m_notation; };

    [[nodiscard]] inline Notations &get_mutable_notation() { return m_notation; };

    [[nodiscard]] inline TimeSignature get_signature() const { return m_signature; };

private:
    TimeSignature m_signature;

    Notations m_notation;
};
