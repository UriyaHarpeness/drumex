#include "TimeSignature.h"

TimeSignature::TimeSignature() = default;

TimeSignature::TimeSignature(unsigned int a, unsigned int b) : Fraction(static_cast<int>(a), static_cast<int>(b),
                                                                        false) {}

Fraction TimeSignature::get_beat() const {
    // todo: may need more complicated or extended logic.
    if (m_value.first % 4 == 0) {
        return *this / 4;
    }
    if (m_value.first % 3 == 0) {
        return *this / 3;
    }
    return Fraction(1, m_value.second);
}
