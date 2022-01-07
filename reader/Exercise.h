#pragma once

#include "../notation/Notation.h"
#include "../variations/Variations.h"
#include "Part.h"
#include "../Translations.h"

#include <fstream>
#include <iostream>
#include <string>
#include <json/json.h>


class Exercise {
public:
    Exercise(const string &path, int index);

    static Part merge_parts(vector<Part> parts, const Json::Value &combination, const Json::Value &signature);

    [[nodiscard]] inline Part &get_part() { return m_part; };

private:
    Part m_part;
};
