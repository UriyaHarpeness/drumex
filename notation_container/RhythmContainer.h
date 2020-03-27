#pragma once

#include "NotationContainer.h"
#include "../time_signature/TimeSignature.h"

#include <set>

using namespace std;

class RhythmContainer : public NotationContainer {
public:
    RhythmContainer(const Locations &locations, const TimeSignature &signature, Instrument rests_location);

    static void find_primes();

    static set<int> get_prime_factors(int value);

    static int get_most_occurring_rhythm(const Locations &locations);

    static Fraction get_beat_from_most_occurring_rhythm(int most_occurring_rhythm, const TimeSignature &signature);

    static bool is_power_of_2(double value);

    static set<int> primes;

private:
    map<Fraction, Locations> m_locations;
};
