#pragma once

#include "Fraction.h"
#include "Notation.h"
#include "../Enums.h"
#include "../display/Display.h"
#include "../metronome/Metronome.h"
#include "../time_signature/TimeSignature.h"

#include <map>
#include <numeric>
#include <cassert>
#include <algorithm>

using namespace std;

namespace NotationUtils {
    /**
     * Unify notation where possible, optimize the notation and remove replaceable clutter.
     *
     * @example 1/4 play + 1/4 rest = 1/2 play.
     * @param notation  Raw notation with plays and rests possibly.
     * @return  The merged notation.
     */
    Voice merge_notation(const Voice &notation);

    Fraction merge_lengths(const vector<Fraction> &lengths);

    vector<Fraction> split_fraction(Fraction fraction, const Fraction &ratio);

    vector<Fraction> split_fraction(const Fraction &beat, Fraction offset, Fraction fraction, const Fraction &ratio);

    int count_remaining_plays(const Voice::iterator &start, const Voice::iterator &end);

    void find_first_non_beamable(Fraction start, const Fraction &end, Voice::iterator &notation_it);

    Voice convert_notation(const Voice &notation, const Fraction &length, const Fraction &beat, const Fraction &ratio);

    Voice generate_voice_notation(const Voice &raw_voice_notation, const TimeSignature &signature);

    Padding merge_padding(const Group &notes);

    Paddings merge_padding(const Paddings &a, const Paddings &b);

    Padding sum_padding(Padding a, Padding b);
};
