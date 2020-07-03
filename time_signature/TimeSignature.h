#pragma once

#include "../notation/Fraction.h"

class TimeSignature : public Fraction {
public:
    TimeSignature();

    TimeSignature(const Fraction &other);

    TimeSignature(unsigned int a, unsigned int b);

    [[nodiscard]] vector<Fraction> get_beats() const;
};

const TimeSignature CommonTime(4, 4);
