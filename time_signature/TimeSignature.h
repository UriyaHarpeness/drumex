#pragma once

#include "../notation/Fraction.h"

class TimeSignature : public Fraction {
public:
    TimeSignature();

    TimeSignature(unsigned int a, unsigned int b);

    [[nodiscard]] Fraction get_beat() const;
};
