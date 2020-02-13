#include "Notation.h"

// todo: this is kind of useless now...
const map<MusicSymbol, pair<MusicSymbolValues, pair<int, int>>>
        Notation::music_symbols_to_values = \
        {{WholeRest,        {SymWholeRest,        {3, 5}}},
         {HalfRest,         {SymHalfRest,         {3, 5}}},
         {QuarterRest,      {SymQuarterRest,      {3, 0}}},
         {EightRest,        {SymEightRest,        {3, 0}}},
         {SixteenthRest,    {SymSixteenthRest,    {3, 0}}},
         {ThirtySecondRest, {SymThirtySecondRest, {3, 0}}},
         {SixtyFourthRest,  {SymSixtyFourthRest,  {3, 0}}}};

// further numbers would be generated, the values should stay as number or this will be endless...
const map<int, MusicSymbol> Notation::rests_to_music_symbols = \
{{0,  WholeRest},
 {-1, HalfRest},
 {-2, QuarterRest},
 {-3, EightRest},
 {-4, SixteenthRest},
 {-5, ThirtySecondRest},
 {-6, SixtyFourthRest}};

const map<Instrument, int> Notation::instrument_to_line = {
        {ChinaInst,    -8},
        {CrashInst,    -6},
        {HiHatInst,    -5},
        {HighTomInst,  -3},
        {MidTomInst,   -2},
        {SnareInst,    -1},
        {MidTomInst,   0},
        {LowTomInst,   1},
        {FloorTomInst, 2},
        {BassInst,     3},
        {UnboundUp,    -3},
        {UnboundDown,  3}
};

const map<Modifier, Padding> Notation::modifier_to_padding = {
        {ModCrossStick, {4,  4}},
        {ModGhost,      {5,  5}},
        {ModAccent,     {0,  0}},
        {ModDot,        {0,  0}},
        {ModRimshot,    {3,  3}},
        {ModFlam,       {18, 0}},
        {ModDrag,       {18, 0}},
        {ModOpenClose,  {2,  2}},
        {ModChoke,      {0,  8}},
};

const map<Modifier, MusicSymbolValues> Notation::modifier_to_symbol = {
        {ModCrossStick, SymCrossStick},
        {ModGhost,      SymGhost},
        {ModAccent,     SymAccent},
        {ModDot,        SymDot},
        {ModRimshot,    SymRimshot},
        {ModFlam,       SymFlam},
        {ModDrag,       SymDrag},
        {ModClose,      SymClose},
        {ModLoose,      SymLoose},
        {ModOpenClose,  SymOpenClose},
        {ModOpen,       SymOpen},
        {ModChoke,      SymChoke},
        {ModRight,      SymRight},
        {ModLeft,       SymLeft},
};

/*
 * Contains 3 ints: offset x, offset y stem length relation, offset y.
 */
const map<Modifier, array<int, 3>> Notation::modifier_to_position = {
        {ModDot,        {0,   0, 0}},
        {ModAccent,     {0,   4, -2}},
        {ModGhost,      {-2,  0, -9}},
        {ModCrossStick, {0,   0, -9}},
        {ModRimshot,    {-2,  0, -9}},
        {ModFlam,       {-15, 0, -10}},
        {ModDrag,       {-15, 0, -11}},
        {ModClose,      {4,   4, -6}},
        {ModLoose,      {4,   4, -6}},
        {ModOpenClose,  {1,   4, -6}},
        {ModOpen,       {4,   4, -6}},
        {ModChoke,      {16,  0, -18}},
        {ModRight,      {-19, 1, 0}},
        {ModLeft,       {-12, 1, 0}},
};

// todo: change to 1/32 after dynamic resizing.
const Fraction Notation::minimal_supported_fraction(1, 16);

shared_ptr<Display> Notation::m_display = nullptr;

Notation::Notation(const Notation &other) = default;

Notation::~Notation() = default;

Notation::Notation(BasicPlaying playing, Instrument instrument, const Fraction &length,
                   const vector<Modifier> &modifiers) : m_line(instrument_to_line.at(instrument)), m_length(length),
                                                        m_instrument(instrument), m_playing(playing),
                                                        m_modifiers(modifiers), m_padding(create_padding(modifiers)) {
    // todo: support 2 whole, etc.

    if (m_playing == BasePlay) {
        // todo: no more than whole note support for now.
        // todo: are music symbol and music symbols values needed?
        // todo: support flam and drag that are not tied to the next note hit (like quick flam of snare and bass)
        if (m_line <= -4) {
            // for cymbals support, temporary i believe in this way.
            m_symbol_value = make_pair(SymCymbal, make_pair(3, 0));
        } else {
            if (Fraction(1, 1) == m_length) {
                m_symbol_value = make_pair(SymWholeNote, make_pair(0, 0));
            } else if (Fraction(1, 2) == m_length) {
                m_symbol_value = make_pair(SymHalfNote, make_pair(3, 0));
            } else {
                m_symbol_value = make_pair(SymQuarterNote, make_pair(3, 0));
            }
        }
    } else {
        // Before generating the standardized notation, this case may be possible.
        if (rests_to_music_symbols.find(static_cast<int>(m_length)) != rests_to_music_symbols.end()) {
            m_symbol = rests_to_music_symbols.at(static_cast<int>(m_length));
            m_symbol_value = music_symbols_to_values.at(m_symbol);
        }
    }
}

void Notation::draw_modifiers(int x, int staff_y, int tail_length) const {
    // currently supports one dot, also may not support more dots in future since more than one makes the notation
    // confusing.

    for (const Modifier &modifier : m_modifiers) {
        const auto position = modifier_to_position.at(modifier);
        // todo: also problematic when one connected notes have this only, plus others.
        if (m_line <= direction_line) {
            m_display->draw_text(modifier_to_symbol.at(modifier), x, staff_y, m_line, position[0],
                                 ((position[1] ? (-(tail_length + position[1]) * line_height) : 0) + position[2]));
        } else {
            m_display->draw_text(modifier_to_symbol.at(modifier), x, staff_y, m_line, position[0],
                                 ((position[1] ? (-(-1 + position[1]) * line_height) : 0) + position[2]));
        }
    }
}

void Notation::draw_flags(int x, int staff_y, int tail_length) const {
    int distance = (m_symbol_value.first == SymCymbal) ? 1 : 0;

    for (int i = 0; i < (-(int) m_length) - 2; i++) {
        if (m_line <= direction_line) {
            m_display->draw_text(SymUpFlag, x + 13,
                                 staff_y + ((m_line - tail_length + (i * 2)) * line_height) - staff_to_0);

        } else {
            m_display->draw_text(SymDownFlag, x + 4,
                                 staff_y + ((m_line + tail_length - (i * 2)) * line_height) - staff_to_0);
        }
    }
}

void Notation::draw_tail(int x, int staff_y, int tail_length) const {
    int distance = (m_symbol_value.first == SymCymbal) ? 1 : 0;
    if (m_line <= direction_line) {
        m_display->draw_rect(x + 13, staff_y + ((m_line - tail_length + 4) * line_height) - staff_to_0,
                             (tail_length + 2 - distance) * line_height - distance, 1);
    } else {
        m_display->draw_rect(x + 4, staff_y + ((m_line + 6) * line_height) - staff_to_0,
                             (tail_length + 2 - distance) * line_height - distance, 1);
    }
}

void Notation::draw_connectors(int x, int staff_y, int line, int length, int number, int tail_length) {
    assert(number > 0);
    bool up = line <= direction_line;
    while (number--) {
        if (up) {
            m_display->draw_rect(x + 13, staff_y + ((line - tail_length + 4) * line_height) - staff_to_0,
                                 connector_height, length);
            line += 2;
        } else {
            m_display->draw_rect(x + 5, staff_y + ((line + tail_length + 7) * line_height) - 2 - staff_to_0,
                                 connector_height, length);
            line -= 2;
        }
    }
}

void Notation::draw_ledgers(int x, int staff_y) const {
    if (m_line <= 0) {
        if (m_line <= -6) {
            m_display->draw_text(SymLedger, x, staff_y + 1 + (-6 * line_height) - staff_to_0);
            if (m_line <= -8) {
                m_display->draw_text(SymLedger, x, staff_y + 1 + (-8 * line_height) - staff_to_0);
            }
        }
    } else {
        if (m_line >= 6) {
            m_display->draw_text(SymLedger, x, staff_y + 1 + (6 * line_height) - staff_to_0);
            if (m_line >= 8) {
                m_display->draw_text(SymLedger, x, staff_y + 1 + (8 * line_height) - staff_to_0);
            }
        }
    }
}

void Notation::draw_head(int x, int staff_y) const {
    m_display->draw_text(m_symbol_value.first, x + m_symbol_value.second.first,
                         staff_y + (m_line * line_height) + m_symbol_value.second.second - staff_to_0);
}

void Notation::display(int x, int staff_y, bool flags, int tail_length) const {
    draw_ledgers(x, staff_y);
    if ((m_length < Fraction(1, 1)) && (m_playing != BaseRest)) {
        if (flags) {
            tail_length = max(tail_length, (((-(int) m_length) - 2) * 2) + 3);
            draw_flags(x, staff_y, tail_length);
        }
        draw_tail(x, staff_y, tail_length);
    }
    draw_modifiers(x, staff_y, tail_length);
    draw_head(x, staff_y);
}

int Notation::calc_needed_space(const Paddings &distances, Fraction offset, Fraction length) {
    int distance;
    Fraction length_end;
    auto position = distances.begin();
    while (position->first <= offset) position++;

    for (distance = 0, length_end = offset + length; position->first <= length_end; distance += (
            position->second[1] + (position + 1)->second[0]), position++);

    return distance;
}

void Notation::draw_connected_notes(int &x, int staff_y, const Paddings &distances, Fraction offset,
                                    const Voice &notations) {
    assert(notations.size() >= 2);

    // Assumes all note are in the same direction.
    int max_height = notations[0][0].get_line();
    int min_height = notations[0][0].get_line();

    int beams = 0, max_beams = 1;
    for (const auto &group : notations) {
        for (const auto &note : group) {
            if (abs(max_height - direction_line) < abs(note.get_line() - direction_line)) {
                max_height = note.get_line();
            }
            if (abs(min_height - direction_line) > abs(note.get_line() - direction_line)) {
                min_height = note.get_line();
            }
            beams = (-2 - static_cast<int>(note.get_rounded_length()));
            if (max_beams < beams) {
                max_beams = beams;
            }
        }
    }

    int line_relation = max(abs(max_height - direction_line - max_beams) - abs(min_height - direction_line), 4) + 3;
    // minimum 1.5 real line difference (1 line from the head end)
    auto position = distances.begin();
    while (position->first <= offset) position++;

    int line, tail_length, distance, last_distance = 0;
    Fraction length_end;
    x += position->second[0];
    for (const auto &group : notations) {
        for (distance = 0, length_end = offset + group[0].get_length(); position->first <= length_end; distance += (
                position->second[1] + (position + 1)->second[0]), position++);
        offset += group[0].get_length();

        for (const auto &note : group) {
            line = note.get_line();
            tail_length = line_relation + (line - min_height);
            note.display(x, staff_y, false, tail_length);
            if (note.get_playing() != BaseRest) {
                // Rests in beams keep the previous note beams number.
                beams = (-2 - (int) note.get_rounded_length());
            }
            if (&note == &(group[0])) {
                if (&group < &(notations[notations.size() - 2])) {
                    draw_connectors(x, staff_y, group[0].get_line(), distance, beams, tail_length);
                } else if (&group == &(notations[notations.size() - 2])) {
                    if (group[0].get_rounded_length() > minimal_supported_fraction) {
                        last_distance = minimal_distance;
                    } else {
                        last_distance = distance / 2;
                    }
                    draw_connectors(x, staff_y, group[0].get_line(), distance - last_distance, beams, tail_length);
                } else if (&group == &(notations[notations.size() - 1])) {
                    // beam note size, or half if must smaller...
                    draw_connectors(x - last_distance, staff_y, group[0].get_line(), last_distance, beams, tail_length);
                }
            }
        }
        x += distance;
    }

    x -= position->second[0];
}

void Notation::draw_individual_notes(int &x, int staff_y, const Paddings &distances, const Fraction &offset,
                                     const Group &group) {
    // todo: assumes all note are in the same direction.

    Fraction length_end;
    auto position = distances.begin();
    while (position->first <= offset) position++;

    x += position->second[0];
    for (const auto &note : group) {
        note.display(x, staff_y, true);
    }
    for (length_end = offset + group[0].get_length();
         position->first <= length_end; x += (position->second[1] + (position + 1)->second[0]), position++);
    x -= position->second[0];
}

Notations Notation::split_voices(const Voice &notation) {
    Voice voice_one;
    Group voice_one_group;
    Group tmp_one;

    Voice voice_two;
    Group voice_two_group;
    Group tmp_two;

    // todo: need to separate sounds and display individually and together at the same time...

    for (const auto &group : notation) {
        for (const auto &note : group) {
            if (note.get_line() > Notation::direction_line) {
                voice_two_group.push_back(note);
                // todo: need to support modifiers in underlying functions.
                voice_one_group.push_back({BaseRest, UnboundUp, note.get_length(), {}});
            } else {
                voice_one_group.push_back(note);
                // todo: need to support modifiers in underlying functions.
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

Voice Notation::merge_notation(const Voice &notation) {
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

Fraction Notation::merge_lengths(const vector<Fraction> &lengths) {
    auto it = lengths.begin();
    Fraction merged_length = *it;

    for (it++; it != lengths.end(); it++) {
        merged_length = (merged_length & *it);
    }

    return merged_length;
}

vector<Fraction> Notation::split_fraction(Fraction fraction) {
    vector<Fraction> fractions;
    Fraction tmp;

    while (fraction) {
        tmp = Fraction(1, pow(2, -(int) fraction));
        if (tmp <= fraction) {
            fractions.push_back(tmp);
            fraction -= tmp;
        }
    }

    return move(fractions);
}

vector<Fraction> Notation::split_fraction(TimeSignature signature, Fraction offset, Fraction fraction) {
    vector<Fraction> fractions;
    vector<Fraction> tmp;

    Fraction bar(signature.first, signature.second);
    Fraction beat(1, signature.second);
    Fraction fill;

    // If it's offset in a beat, first filling the beat is needed before stretching over it.
    if (static_cast<bool>(offset % beat)) {
        fill = (fraction <= (beat - (offset % beat))) ? fraction : (beat - (offset % beat));
        tmp = split_fraction(fill);
        fractions.insert(fractions.end(), tmp.begin(), tmp.end());

        offset += fill;
        fraction -= fill;
    }

    while (fraction) {
        fill = (fraction <= (bar - (offset % bar))) ? fraction : (bar - (offset % bar));

        tmp = split_fraction(fill);
        fractions.insert(fractions.end(), tmp.begin(), tmp.end());

        offset += fill;
        fraction -= fill;
    }

    return move(fractions);
}

Notations Notation::split_notation(const Voice &notation, const Fraction &bar) {
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

int Notation::count_remaining_plays(Fraction offset, const Fraction &beat, Voice::const_iterator notation_it) {
    int count = 0;

    // warning; optional segfault, iterates through the iterator without knowing there it ends, the callet should be
    // aware.
    while (offset < beat) {
        notation_it++;
        offset += (*notation_it)[0].get_length();
        count += (*notation_it)[0].get_playing() == BasePlay ? 1 : 0;
    }

    return count;
}

Notations
Notation::connect_notation(const Voice &notation, const Fraction &beat) {
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
        // todo: should if any is playing, although if one is playing than there's no rest...
        if ((group[0].get_playing() == BasePlay) && (group[0].get_length() < beat)) {
            connecting = true;
        }
        part_notation.push_back(group);
        if ((((group_it + 1 == notation.end()) ? 0 : count_remaining_plays(length_sum, beat, group_it))) == 0) {
            connecting = false;
        }
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

Voice Notation::convert_notation(const Voice &notation, TimeSignature signature) {
    Voice generated_notation;

    Fraction offset(0, 1);
    Fraction bar(signature);

    for (const auto &group : notation) {
        // assumes all the group has the same BasicPlaying and length
        auto fractions = split_fraction(signature, offset, group[0].get_rounded_length());
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
                    auto note = generated_notation[generated_notation.size() - 1][0];
                    if ((note.get_playing() == BasePlay) &&
                        (find(note.get_modifiers().begin(), note.get_modifiers().end(), ModDot) ==
                         note.get_modifiers().end()) && (note.get_length() / fraction == Fraction(2, 1)) &&
                        ((offset % bar) + note.get_length() < bar)) {
                        dot = true;
                    }
                }
                if (dot) {
                    for (auto &note : generated_notation[generated_notation.size() - 1]) {
                        note.add_modifier(ModDot);
                    }
                } else {
                    generated_notation.push_back({{BaseRest, (group[0].get_line() > direction_line) ? UnboundDown
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

Voice
Notation::generate_voice_notation(const Voice &raw_voice_notation, TimeSignature signature) {
    Voice merged_stuff = merge_notation(raw_voice_notation);

    return move(convert_notation(merged_stuff, signature));
}

Notations Notation::generate_notation(const Voice &raw_notation, TimeSignature signature) {
    Notations notation = split_voices(raw_notation);

    Notations generated_notation =
            {generate_voice_notation(notation[0], signature), generate_voice_notation(notation[1], signature)};

    return move(generated_notation);
}

void Notation::stretch_notation(Notations &notation, Fraction old_length, Fraction new_length) {
    const Notations original = notation;

    for (; new_length != old_length; new_length -= old_length) {
        notation[0].insert(notation[0].end(), original[0].begin(), original[0].end());
        notation[1].insert(notation[1].end(), original[1].begin(), original[1].end());
    }
}

Padding Notation::create_padding(const vector<Modifier> &modifiers) {
    Padding padding = {0, 0};

    for (const auto &modifier : modifiers) {
        auto got_padding = modifier_to_padding.find(modifier);
        if (got_padding != modifier_to_padding.end()) {
            padding = merge_padding(padding, got_padding->second);
        }
    }

    return padding;
}

void Notation::insert_padding(Paddings &paddings, const Fraction &offset, Padding padding) {
    auto padding_iterator = paddings.begin();
    while ((padding_iterator != paddings.end()) && (padding_iterator->first < offset)) padding_iterator++;

    if ((padding_iterator != paddings.end()) && (padding_iterator->first == offset)) {
        padding_iterator->second = merge_padding(padding_iterator->second, padding);
    } else {
        paddings.insert(padding_iterator, {offset, padding});
    }
}

Padding Notation::merge_padding(const Padding &a, const Padding &b) {
    Padding padding = {0, 0};

    for (int i = 0; i < tuple_size<Padding>::value; i++) {
        padding[i] = max(a[i], b[i]);
    }

    return padding;
}

Padding Notation::merge_padding(const Group &notes) {
    Padding padding = {0, 0};

    for (const Notation &note : notes) {
        padding = merge_padding(padding, note.get_padding());
    }

    return padding;
}

Fraction Notation::sum_length(const Voice &notes) {
    Fraction length;

    for (const auto &note_group : notes) {
        length += note_group[0].get_length();
    }

    return length;
}

Fraction Notation::sum_length(const Notations &notes) {
    Fraction length;

    for (const auto &connected_notes : notes) {
        length += sum_length(connected_notes);
    }

    return length;
}

Fraction Notation::sum_final_length(const Notations &notes) {
    Fraction length;

    // Assumes that at this point the lengthes of the voices are equal.
    for (const auto &group : notes[0]) {
        length += group[0].get_length();
    }

    return length;
}

void Notation::display_notation(const GroupedNotations &notation, const Paddings &distances, const Fraction &bar,
                                const int played_line) {
    //d.draw_base(3, 16);
    /*for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            s[0] = (char) (i * 16 + j);
            if (s[0] == 0)continue;

            //d.draw_image("../stuff.png", 400, 0, a * 8, a * 8);
            //d.draw_rect(a, a, 80, 10);

            string k(s);
            d->draw_text(k, 40 + (j * 60), 100 + (i * 60));
            //d.draw_text(to_string(i * 16 + j), 100, 100);
        }
    }*/

    int off_x, off_y, edge_padding = 20, lines;
    Fraction offset;
    m_display->draw_base(20, displaying_init_y, 3, 4);

    // todo: skip right to displayed notes, without calculating the distance all over again.
    for (const auto &voice : notation) {
        lines = 0;
        offset = {0, 1};
        off_x = displaying_init_x;
        off_y = displaying_init_y;
        for (const Voice &note_groups : voice) {
            // Move to the next line to avoid displaying over the staff.
            if (off_x + calc_needed_space(distances, offset, sum_length(note_groups)) > 800 - edge_padding) {
                off_x = displaying_init_x;
                off_y += staff_lines_spacing;
                lines++;
                if (lines >= (played_line + max_lines_displayed)) {
                    // Display up to 4 lines.
                    break;
                }
                if ((lines % max_lines_displayed) == 0) {
                    off_y = displaying_init_y;
                }

                m_display->draw_base(edge_padding, off_y, 3, 4);
            }

            if (lines < played_line) {
                off_x += calc_needed_space(distances, offset, sum_length(note_groups));
            } else {
                if (note_groups.size() > 1) {
                    draw_connected_notes(off_x, off_y, distances, offset, note_groups);
                } else {
                    draw_individual_notes(off_x, off_y, distances, offset, note_groups[0]);
                }
            }

            // The notes do not stretch over bars, so summing the offset will not miss bars.
            offset += sum_length(note_groups);

            if (!static_cast<bool>(offset % bar)) {
                off_x += 10;
                m_display->draw_text(SymBarLine, off_x, off_y);
                off_x += 10;
            }
            // printing numbers works great.
        }
    }
}

pair<pair<int, int>, Padding>
Notation::get_note_location(const GroupedNotations &notation, const Paddings &distances, const Fraction &bar,
                            const Fraction &location) {
    // todo: optimize this function.
    int off_x = displaying_init_x, off_y = displaying_init_y, edge_padding = 20;
    Padding padding;
    Fraction offset;

    for (const Voice &note_groups : notation[0]) {
        // Move to the next line to avoid displaying over the staff.
        if (off_x + calc_needed_space(distances, offset, sum_length(note_groups)) > 800 - edge_padding) {
            off_x = displaying_init_x;
            off_y += staff_lines_spacing;
        }

        int distance;
        Fraction length_end;
        auto position = distances.begin();
        while (position->first <= offset) position++;

        if (offset + sum_length(note_groups) > location) {
            for (length_end = location;
                 position->first <= length_end; off_x += (position->second[0] + position->second[1]), position++);
            padding = (position)->second;
            break;
        }

        int tmp_x = 0;
        tmp_x += position->second[0];
        for (distance = 0, length_end = offset + sum_length(note_groups); position->first <= length_end; distance += (
                position->second[1] + (position + 1)->second[0]), position++);
        tmp_x += distance;
        tmp_x -= position->second[0];

        off_x += tmp_x;

        offset += sum_length(note_groups);

        if (!static_cast<bool>(offset % bar)) {
            off_x += 20;
        }
    }

    return {{off_x, off_y}, padding};
}

void
Notation::continuous_display_notation(const GroupedNotations &notation, const Paddings &distances, const Fraction &bar,
                                      int tempo) {
    vector<Fraction> locations;
    for (const auto &i : distances) {
        locations.push_back(i.first);
    }

    Metronome m(move(locations), tempo, {1, bar.get_value().second});
    while (true) {
        // todo: be responsive to SDL events, to avoid the annoying pop-up.
        cout << *m.get_current_location() << endl;

        auto note_location = get_note_location(notation, distances, bar, *m.get_current_location());

        const int played_line = (note_location.first.second - displaying_init_y) / staff_lines_spacing;

        m_display->clear_screen();

        m_display->draw_rect(note_location.first.first + 1,
                             ((played_line % max_lines_displayed) * staff_lines_spacing) + displaying_init_y - 20,
                             line_height * 16, get<0>(note_location.second) + get<1>(note_location.second), 255, 64, 64,
                             128);

        display_notation(notation, distances, bar, played_line);

        m_display->present();
        m.poll();
    }
}

void Notation::prepare_displayable_notation(const Notations &generated_notation, GroupedNotations &notation,
                                            Paddings &distances, const Fraction &bar) {
    Fraction beat = {1, bar.get_value().second};

    Notations splitted_notation = split_notation(generated_notation[0], bar);

    Notations voice_notation;
    Notations small_connected_notation;

    for (const Voice &small_notation : splitted_notation) {
        small_connected_notation = connect_notation(small_notation, beat);
        voice_notation.insert(voice_notation.end(), small_connected_notation.begin(), small_connected_notation.end());
    }
    notation.push_back(move(voice_notation));
    voice_notation.clear();
    splitted_notation = split_notation(generated_notation[1], bar);

    for (const Voice &small_notation : splitted_notation) {
        small_connected_notation = connect_notation(small_notation, beat);
        voice_notation.insert(voice_notation.end(), small_connected_notation.begin(), small_connected_notation.end());
    }
    notation.push_back(move(voice_notation));
    voice_notation.clear();

    // asserts two voices are same offset, when using one voice don't use this.
    assert(sum_length(notation[0]) == sum_length(notation[1]));

    Paddings merged_padding;
    // maybe typedef Paddings as Distances

    // todo: maybe simply use 0,1 for default constructor and be over with all this initialization stuff.
    // todo: think what to do with notes with value below minimum supported fraction on different lines.
    Fraction notes_offset;
    for (const auto &notes : generated_notation[0]) {
        insert_padding(merged_padding, notes_offset, merge_padding(notes));
        notes_offset += notes[0].get_length();
    }
    insert_padding(merged_padding, notes_offset, {0, 0});
    insert_padding(merged_padding, notes_offset + bar, {0, 0});

    notes_offset = {0, 1};
    for (const auto &notes : generated_notation[1]) {
        insert_padding(merged_padding, notes_offset, merge_padding(notes));
        notes_offset += notes[0].get_length();
    }
    insert_padding(merged_padding, notes_offset, {0, 0});
    insert_padding(merged_padding, notes_offset + bar, {0, 0});

    pair<Fraction, Padding> previous;
    for (const auto &padding : merged_padding) {
        if (padding.first) {
            distances.emplace_back(padding.first, get_distance(padding.first - previous.first, previous.second));
        } else {
            distances.emplace_back(padding.first, get_distance({}, previous.second));
        }
        previous = padding;
    }
}

Fraction Notation::get_length() const {
    return ((m_length * 2) - (m_length * Fraction(1, pow(2, count(m_modifiers.begin(), m_modifiers.end(), ModDot)))));
}

void Notation::reset_length(const Fraction &length) {
    m_length = length;
    auto location = find(m_modifiers.begin(), m_modifiers.end(), ModDot);
    if (location != m_modifiers.end()) {
        m_modifiers.erase(location);
    }
}
