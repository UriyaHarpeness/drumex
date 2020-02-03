#pragma once

#include "../notation/Notation.h"
#include "../variations/Variations.h"
#include "Part.h"
#include "../Translations.h"

#include <fstream>
#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>


class Exercise {
public:
    Exercise(const string &path, int index);

    [[nodiscard]] inline const vector<Part> &get_parts() const { return m_parts; };

private:
    vector<Part> m_parts;
};
