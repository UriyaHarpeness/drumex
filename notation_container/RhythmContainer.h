#pragma once

#include "NotationContainer.h"
#include "../time_signature/TimeSignature.h"

#include <set>

using namespace std;

class RhythmContainer : public NotationContainer {
public:
    RhythmContainer(const Locations &locations, const TimeSignature &scope, Instrument rests_location,
                    const Fraction &offset,
                    const Fraction& ratio);

    static void find_primes();

    static set<int> get_prime_factors(int value);

    static pair<int, bool> get_most_occurring_rhythm(const Locations &locations);

    static Fraction get_beat_from_most_occurring_rhythm(int most_occurring_rhythm, const TimeSignature &signature);

    static bool is_power_of_2(double value);

    static set<int> primes;

private:
    Locations m_locations;

    Fraction m_offset;

    Fraction m_ratio;
};
