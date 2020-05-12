#pragma once

#include "../location/Location.h"
#include "../time_signature/TimeSignature.h"
#include "../notation/NotationUtils.h"
#include "../notation/NotationDisplay.h"

#include <set>

using namespace std;

class RhythmContainer {
public:
    RhythmContainer(const Locations &locations, const TimeSignature &scope, Instrument rests_location,
                    const Fraction &offset, const Fraction &ratio);

    void optimize();

    void notationize(Instrument rests_location);

    void beam();

    void prepare_padding(Paddings &padding, int start_padding = 20, int end_padding = 20) const;

    void
    display(const GlobalLocations &global_locations, const int y, int start_padding = 20, int end_padding = 20) const;

    void extend(const RhythmContainer &container);

    static void find_primes();

    static set<int> get_prime_factors(int value);

    static pair<int, bool> get_most_occurring_rhythm(const Locations &locations);

    static Fraction get_beat_from_most_occurring_rhythm(int most_occurring_rhythm, const TimeSignature &signature,
                                                        const Fraction &ratio);


    static bool is_power_of_2(double value);

    [[nodiscard]] inline vector<RhythmContainer> &get_mutable_rhythms() { return m_rhythms_containers; };

    [[nodiscard]] inline const Voice &get_notations() const { return m_notations; };

    [[nodiscard]] inline const BeamedVoice &get_beamed_notations() const { return m_beamed_notations; };

    [[nodiscard]] inline const Fraction &get_offset() const { return m_offset; };

    static set<int> primes;

private:
    Locations m_locations;

    vector<RhythmContainer> m_rhythms_containers;

    Fraction m_offset;

    Fraction m_ratio;

    Fraction m_length;

    Fraction m_beat;

    int m_most_occurring_rhythm;

    Voice m_notations;

    BeamedVoice m_beamed_notations;
};
