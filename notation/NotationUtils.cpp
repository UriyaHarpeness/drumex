#include "NotationUtils.h"

Notations NotationUtils::split_voices(const Voice &notation) {
    Voice voice_one;
    Group voice_one_group;
    Group tmp_one;

    Voice voice_two;
    Group voice_two_group;
    Group tmp_two;

    for (const auto &group : notation) {
        for (const auto &note : group) {
            if (note.get_line() > DisplayConstants::direction_line) {
                voice_two_group.push_back(note);
                voice_one_group.push_back({BaseRest, UnboundUp, note.get_length(), {}});
            } else {
                voice_one_group.push_back(note);
                voice_two_group.push_back({BaseRest, UnboundDown, note.get_length(), {}});
            }
        }
        assert(!voice_one_group.empty());
        assert(!voice_two_group.empty());

        for (const auto &note : voice_one_group) {
            if (note.get_playing() == BasePlay) {
                tmp_one.push_back(note);
            }
        }
        if (tmp_one.empty()) {
            tmp_one.push_back(voice_one_group[0]);
        }
        voice_one.push_back(move(tmp_one));
        voice_one_group.clear();
        tmp_one.clear();

        for (const auto &note : voice_two_group) {
            if (note.get_playing() == BasePlay) {
                tmp_two.push_back(note);
            }
        }
        if (tmp_two.empty()) {
            tmp_two.push_back(voice_two_group[0]);
        }
        voice_two.push_back(move(tmp_two));
        voice_two_group.clear();
        tmp_two.clear();
    }

    return {move(voice_one), move(voice_two)};
}

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

Notations NotationUtils::split_notation(const Voice &notation, const Fraction &bar) {
    Notations splitted_notation;
    Voice part_notation;
    Fraction length_sum;

    // This function ignores ModDot since it will be decided when generating the notation.
    // Also, this code assumes (which is a reasonable assumption) that every group has the same BasicPlaying.

    for (const auto &group : notation) {
        length_sum += group[0].get_length();
        part_notation.push_back(group);
        assert(length_sum <= bar);
        if (length_sum == bar) {
            splitted_notation.push_back(move(part_notation));
            part_notation.clear();
            length_sum = {0, 1};
        }
    }

    assert(part_notation.empty());

    return move(splitted_notation);
}

int NotationUtils::count_remaining_plays(const Voice::iterator &start, const Voice::iterator &end) {
    int count = 0;

    for (auto it = start; it != end; it++) {
        cout << (*it)[0] << endl;
        count += (*it)[0].get_playing() == BasePlay ? 1 : 0;
    }

    return count;
}

void NotationUtils::find_first_non_beamable(Fraction start, const Fraction &end, Voice::iterator &notation_it) {
    // warning; optional segfault, iterates through the iterator without knowing there it ends, the caller should be
    // aware.
    cout << start << " - " << end << endl;
    while (start < end) {
        if ((*notation_it)[0].get_simple_length() >= Fraction(1, 4)) {
            break;
        }
        start += (*notation_it)[0].get_length();
        notation_it++;
        cout << start << " = " << end << endl;
    }
}

Notations NotationUtils::connect_notation(const Voice &notation, const Fraction &beat) {
    Notations connected_notation;
    Voice part_notation;
    Fraction length_sum;

    // This function ignores ModDot since it will be decided when generating the notation.
    // Also, this code assumes (which is a reasonable assumption) that every group has the same BasicPlaying.

    // Beaming needs to be done inside a single beat, only between the first and last played note if there are at least
    // two, also includes the rests between them.
    bool connecting = false;
    for (auto group_it = notation.begin(); group_it != notation.end(); group_it++) {
        const auto &group = *group_it;
        length_sum += group[0].get_length();
        if ((group[0].get_playing() == BasePlay) && (group[0].get_length() < beat)) {
            connecting = true;
        }
        part_notation.push_back(group);
        //if ((((group_it + 1 == notation.end()) ? 0 : count_remaining_plays(length_sum, beat, group_it))) == 0) {
        //    connecting = false;
        //}
        if (!connecting) {
            connected_notation.push_back(move(part_notation));
            part_notation.clear();
        }
        if (length_sum >= beat) {
            length_sum = length_sum % beat;
        }
    }

    assert(part_notation.empty());

    return move(connected_notation);
}

Voice NotationUtils::convert_notation(const Voice &notation, const Fraction &length, const Fraction &beat,
                                      const Fraction &ratio) {
    Voice generated_notation;

    Fraction offset;

    for (const auto &group : notation) {
        // assumes all the group has the same BasicPlaying and length
        // todo: need to think if possible or enable a group that contains note with different ratio (logically isn't rational, it should be splitted beteen voices).
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
                // todo: no modifiers for rests, check for playing.

                // Add ModDot if possible.
                bool dot = false;
                if (!generated_notation.empty()) {
                    const auto &note = generated_notation[generated_notation.size() - 1][0];
                    const auto &modifiers = note.get_modifiers();

                    if ((note.get_playing() == BasePlay) &&
                        (find(modifiers.begin(), modifiers.end(), ModDot) == modifiers.end()) &&
                        (note.get_length() / fraction == Fraction(2, 1)) &&
                        ((offset % length) + note.get_length() < length)) {
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


    // todo: will not work with 1/3 on 4/4 for example... need to support tuplets and stuff...
    // plus problematic if a note is as long as several bars, also does not consider other notes that may break the consistency.
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

    return merged_stuff; // move(convert_notation(merged_stuff, signature));
}

Notations NotationUtils::generate_notation(const Voice &raw_notation, const TimeSignature &signature) {
    Notations notation = split_voices(raw_notation);

    Notations generated_notation =
            {generate_voice_notation(notation[0], signature), generate_voice_notation(notation[1], signature)};

    return move(generated_notation);
}

void NotationUtils::stretch_notation(Notations &notation, const Fraction &old_length, Fraction new_length) {
    const Notations original = notation;

    for (; new_length != old_length; new_length -= old_length) {
        notation[0].insert(notation[0].end(), original[0].begin(), original[0].end());
        notation[1].insert(notation[1].end(), original[1].begin(), original[1].end());
    }
}

void NotationUtils::insert_padding(Paddings &paddings, const Fraction &offset, Padding padding) {
    auto padding_iterator = paddings.begin();
    while ((padding_iterator != paddings.end()) && (padding_iterator->first < offset)) padding_iterator++;

    if ((padding_iterator != paddings.end()) && (padding_iterator->first == offset)) {
        padding_iterator->second = Notation::merge_padding(padding_iterator->second, padding);
    } else {
        paddings.insert(padding_iterator, {offset, padding});
    }
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

Fraction NotationUtils::sum_length(const Voice &notes) {
    Fraction length;

    for (const auto &note_group : notes) {
        length += note_group[0].get_length();
    }

    return length;
}

Fraction NotationUtils::sum_length(const Notations &notes) {
    Fraction length;

    for (const auto &connected_notes : notes) {
        length += sum_length(connected_notes);
    }

    return length;
}

Fraction NotationUtils::sum_final_length(const Notations &notes) {
    Fraction length;

    // Assumes that at this point the lengths of the voices are equal.
    for (const auto &group : notes[0]) {
        length += group[0].get_length();
    }

    return length;
}
