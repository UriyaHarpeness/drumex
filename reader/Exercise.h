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

    [[nodiscard]] inline const Part &get_part() const { return m_part; };

private:
    Part m_part;
};
