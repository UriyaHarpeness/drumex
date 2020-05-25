#include "NotationUtils.h"

Voice NotationUtils::merge_notation(const Voice &notation) {
    Voice merged_notation = {notation[0]};

    // This function ignores ModDot since it will be decided when generating the notation.
    // Also, this code assumes (which is a reasonable assumption) that every group has the same BasicPlaying.

    auto group = notation.begin();
    for (group++; group != notation.end(); group++) {
        if ((*group)[0].get_playing() == BasePlay) {
            merged_notation.push_back(*group);
        } else {
            for (auto &single : merged_notation[merged_notation.size() - 1]) {
                single.set_rounded_length(single.get_rounded_length() + (*group)[0].get_rounded_length());
            }
        }
    }

    return move(merged_notation);
}

Fraction NotationUtils::merge_lengths(const vector<Fraction> &lengths) {
    auto it = lengths.begin();
    Fraction merged_length = *it;

    for (it++; it != lengths.end(); it++) {
        merged_length = (merged_length & *it);
    }

    return merged_length;
}

vector<Fraction> NotationUtils::split_fraction(Fraction fraction, const Fraction &ratio) {
    vector<Fraction> fractions;
    Fraction tmp;

    fraction = fraction / ratio;
    while (fraction) {
        tmp = Fraction::FloatingFraction(1, pow(2, -static_cast<int>(fraction)));
        if (tmp <= fraction) {
            fractions.push_back(tmp * ratio);
            fraction -= tmp;
        }
    }

    return move(fractions);
}

vector<Fraction>
NotationUtils::split_fraction(const Fraction &beat, Fraction offset, Fraction fraction, const Fraction &ratio) {
    vector<Fraction> fractions;
    vector<Fraction> tmp;

    Fraction fill;

    // If it's offset in a beat, first filling the beat is needed before stretching over it.
    if (static_cast<bool>(offset % beat)) {
        fill = min(fraction, beat - (offset % beat));
        tmp = split_fraction(fill, ratio);
        fractions.insert(fractions.end(), tmp.begin(), tmp.end());

        offset += fill;
        fraction -= fill;
    }

    // After using the BarContainer, the note will never overlaps the bar.
    tmp = split_fraction(fraction, ratio);
    fractions.insert(fractions.end(), tmp.begin(), tmp.end());

    return move(fractions);
}

int NotationUtils::count_remaining_plays(const Voice::iterator &start, const Voice::iterator &end) {
    int count = 0;

    for (auto it = start; it != end; it++) {
        count += (*it)[0].get_playing() == BasePlay ? 1 : 0;
    }

    return count;
}

void NotationUtils::find_first_non_beamable(Fraction start, const Fraction &end, Voice::iterator &notation_it) {
    // Warning: possible segfault, iterates through the iterator without knowing there it ends, the caller should be
    // aware.
    while (start < end) {
        if ((*notation_it)[0].get_simple_length() >= Fraction(1, 4)) {
            break;
        }
        start += (*notation_it)[0].get_length();
        notation_it++;
    }
}

Voice NotationUtils::convert_notation(const Voice &notation, const Fraction &length, const Fraction &beat,
                                      const Fraction &ratio) {
    Voice generated_notation;

    Fraction offset;

    for (const auto &group : notation) {
        // Assumes all the group has the same BasicPlaying and length.
        auto fractions = split_fraction(beat, offset, group[0].get_rounded_length(), ratio);
        BasicPlaying playing = group[0].get_playing();
        for (const auto &fraction : fractions) {
            if (playing == BasePlay) {
                Group tmp;
                for (const auto &note : group) {
                    tmp.emplace_back(BasePlay, note.get_instrument(), fraction, note.get_modifiers());
                }
                generated_notation.push_back(move(tmp));
                tmp.clear();
            } else {
                // Add ModDot if possible.
                bool dot = false;
                if (!generated_notation.empty()) {
                    const auto &note = generated_notation[generated_notation.size() - 1][0];
                    const auto &modifiers = note.get_modifiers();

                    if ((note.get_playing() == BasePlay) &&
                        (find(modifiers.begin(), modifiers.end(), ModDot) == modifiers.end()) &&
                        (note.get_length() / fraction == Fraction(2, 1)) &&
                        (((offset % beat) + fraction + note.get_length() <= beat) ||
                         (!static_cast<bool>(offset % beat) && !static_cast<bool>(note.get_length() % beat)))) {
                        dot = true;
                    }
                }
                if (dot) {
                    for (auto &note : generated_notation[generated_notation.size() - 1]) {
                        note.add_modifier(ModDot);
                    }
                } else {
                    generated_notation.push_back(
                            {{BaseRest, (group[0].get_line() > DisplayConstants::direction_line) ? UnboundDown
                                                                                                 : UnboundUp, fraction, {}}});
                }
            }
            playing = BaseRest;
        }
        offset += group[0].get_rounded_length();
    }


    // todo: problematic if a note is as long as several bars, also does not consider other notes that may break the consistency.
    /*
     * Irrational notes are rounded up for the notes used, for example: 2/3 would contain 2-1/2, 3/7 would contain 3-1/4.
     *
     * Irrational note values are not used when not in a context of other note values, 3/5 is the same as 3/4 if the
     * whole piece keeps the same signature...
     */

    // also support 2 whole, 4 whole, etc.

    return move(generated_notation);
}

Voice NotationUtils::generate_voice_notation(const Voice &raw_voice_notation, const TimeSignature &signature) {
    Voice merged_stuff = merge_notation(raw_voice_notation);

    return merged_stuff;
}

Padding NotationUtils::merge_padding(const Group &notes) {
    Padding padding = {0, 0};

    for (const Notation &note : notes) {
        padding = Notation::merge_padding(padding, note.get_padding());
    }

    return move(padding);
}

Paddings NotationUtils::merge_padding(const Paddings &a, const Paddings &b) {
    Paddings padding = a;

    for (const auto &it : b) {
        padding[it.first] = Notation::merge_padding(padding[it.first], it.second);
    }

    return padding;
}

Padding NotationUtils::sum_padding(Padding a, Padding b) {
    for (size_t i = 0; i < a.size(); i++) {
        a[i] += b[i];
    }
    return a;
}
