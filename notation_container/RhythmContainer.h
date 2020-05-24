#pragma once

#include "../location/Location.h"
#include "../time_signature/TimeSignature.h"
#include "../notation/NotationUtils.h"
#include "../notation/NotationDisplay.h"
#include "../logging/Logger.h"

#include <set>

using namespace std;

enum NotesDirection {
    NotesUp,
    NotesDown
};

class RhythmContainer {
public:
    RhythmContainer(const Locations &locations, const TimeSignature &scope, NotesDirection direction,
                    const Fraction &offset, const Fraction &ratio);

    void optimize();

    void notationize();

    void beam();

    void prepare_empty_padding(Paddings &padding) const;

    void fill_padding(Paddings &padding, int start_padding = 20, int end_padding = 20) const;

    void display(const GlobalLocations &global_locations, int y, int start_padding = 20, int end_padding = 20) const;

    void extend(const RhythmContainer &container);

    void init_display_scope();

    static void find_primes();

    static set<int> get_prime_factors(int value);

    static pair<int, bool> get_most_occurring_rhythm(const Locations &locations);

    static Fraction get_beat_from_most_occurring_rhythm(int most_occurring_rhythm, const TimeSignature &signature,
                                                        const Fraction &ratio);

    [[nodiscard]] inline vector<RhythmContainer> &get_mutable_rhythms() { return m_rhythms_containers; };

    [[nodiscard]] inline const Voice &get_notations() const { return m_notations; };

    [[nodiscard]] inline const BeamedVoice &get_beamed_notations() const { return m_beamed_notations; };

    [[nodiscard]] inline const Fraction &get_offset() const { return m_offset; };

    [[nodiscard]] inline const int &get_max_used_line() const { return m_max_used_line; };

    [[nodiscard]] inline const int &get_min_used_line() const { return m_min_used_line; };

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

    NotesDirection m_direction;

    int m_max_used_line;

    int m_min_used_line;
};
