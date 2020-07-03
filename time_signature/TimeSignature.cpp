#include "TimeSignature.h"

TimeSignature::TimeSignature() = default;

TimeSignature::TimeSignature(const Fraction &other) : Fraction(other.get_value().first, other.get_value().second,
                                                               false) {}

TimeSignature::TimeSignature(unsigned int a, unsigned int b) : Fraction(static_cast<int>(a), static_cast<int>(b),
                                                                        false) {}

vector<Fraction> TimeSignature::get_beats() const {
    /**
     * The order of deciding on beats is as follows:
     * 1.   If it divides by 1/4 - all beats of 1/4.
     * 2.   If it divides by 1/3 - all beats of 1/3.
     * 3.   Otherwise - beats of 1/4 except the leftover (which will be below 1/4).
     */
    vector<Fraction> beats;

    if (!static_cast<bool>(*this % Fraction(1, 4))) {
        beats.resize((int) static_cast<double>(*this / Fraction(1, 4)), Fraction(1, 4));
        return beats;
    }
    if (!static_cast<bool>(*this % Fraction(3, 8))) {
        beats.resize((int) static_cast<double>(*this / Fraction(3, 8)), Fraction(3, 8));
        return beats;
    }
    auto value = *this;
    do {
        auto beat = min(Fraction(1, 4), Fraction(value));
        beats.push_back(beat);
        value -= beat;
    } while (static_cast<bool>(value));
    return beats;
}
