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
    Notations split_voices(const Voice &notation);

    /**
     * Unify notation where possible, optimize the notation and remove replaceable clutter.
     *
     * @example 1/4 play + 1/4 rest = 1/2 play.
     * @param notation  Raw notation with plays and rests possibly.
     * @return  The merged notation.
     */
    Voice merge_notation(const Voice &notation);

    Fraction merge_lengths(const vector<Fraction> &lengths);

    vector<Fraction> split_fraction(Fraction fraction);

    vector<Fraction> split_fraction(const TimeSignature &signature, Fraction offset, Fraction fraction);

    Notations split_notation(const Voice &notation, const Fraction &bar);

    int count_remaining_plays(Fraction offset, const Fraction &beat, Voice::const_iterator notation_it);

    Notations connect_notation(const Voice &notation, const Fraction &beat);

    Voice convert_notation(const Voice &notation, const TimeSignature &signature);

    Voice generate_voice_notation(const Voice &raw_voice_notation, TimeSignature signature);

    Notations generate_notation(const Voice &raw_notation, TimeSignature signature);

    void stretch_notation(Notations &notation, const Fraction &old_length, Fraction new_length);

    void insert_padding(Paddings &paddings, const Fraction &offset, Padding padding);

    Padding merge_padding(const Group &notes);

    Fraction sum_length(const Voice &notes);

    Fraction sum_length(const Notations &notes);

    Fraction sum_final_length(const Notations &notes);
};
