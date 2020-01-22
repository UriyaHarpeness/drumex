#include "Notation.h"

// todo: this is kind of useless now...
const map<MusicSymbol, pair<MusicSymbolValues, pair<int, int>>>
        Notation::music_symbols_to_values = \
        {{WholeRest,        {SymWholeRest,        {3, 10}}},
         {HalfRest,         {SymHalfRest,         {3, 10}}},
         {QuarterRest,      {SymQuarterRest,      {3, 0}}},
         {EightRest,        {SymEightRest,        {3, 0}}},
         {SixteenthRest,    {SymSixteenthRest,    {3, 0}}},
         {ThirtySecondRest, {SymThirtySecondRest, {3, 0}}}};

// further numbers would be generated, the values should stay as number or this will be endless...
const map<int, MusicSymbol> Notation::rests_to_music_symbols = \
{{0,  WholeRest},
 {-1, HalfRest},
 {-2, QuarterRest},
 {-3, EightRest},
 {-4, SixteenthRest},
 {-5, ThirtySecondRest}};

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
        {UnboundUp,    -4},
        {UnboundDown,  4}
};

const map<Modifier, Padding> Notation::modifier_to_padding = {
        {ModCrossStick, {4,  minimal_distance + 4}},
        {ModGhost,      {5,  minimal_distance + 5}},
        {ModAccent,     {0,  minimal_distance}},
        {ModDot,        {0,  minimal_distance}},
        {ModRimshot,    {3,  minimal_distance + 3}},
        {ModFlam,       {18, minimal_distance}},
        {ModDrag,       {18, minimal_distance}},
        {ModOpenClose,  {2,  minimal_distance + 2}},
        {ModChoke,      {0,  minimal_distance + 8}},
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
};

/*
 * Contains 3 ints: offset x, offset y, offset y stem length relation.
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
};

// todo: change to 1/32 after dynamic resizing.
const Fraction Notation::minimal_supported_fraction(1, 16);

shared_ptr<Display> Notation::m_display = nullptr;

Notation::Notation(const Notation &other) : m_symbol(other.m_symbol), m_symbol_value(other.m_symbol_value),
                                            m_line(other.m_line), m_length(other.m_length),
                                            m_instrument(other.m_instrument),
                                            m_playing(other.m_playing), m_modifiers(other.m_modifiers),
                                            m_padding(other.m_padding) {
}

Notation::Notation(BasicPlaying playing, Instrument instrument, Fraction length, vector<Modifier> modifiers) :
        m_line(instrument_to_line.at(instrument)), m_length(length), m_instrument(instrument), m_playing(playing),
        m_modifiers(modifiers), m_padding(create_padding(modifiers)) {
    // todo: support 2 whole, etc.
    //m_symbol = playing_to_music_symbols.at(playing).at(length);
    if (m_playing == BasePlay) {
        // todo: no more than whole note support for now.
        // todo: are music symbol and music symbols values needed?
        // todo: support flam and drag that are not tied to the next note hit (like quick flam of snare and bass)
        if (m_line <= -4) {
            //for cymbals support, temporary i believe in this way.
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
        m_symbol = rests_to_music_symbols.at(m_length);
        m_symbol_value = music_symbols_to_values.at(m_symbol);
    }
}

Notation::~Notation() {
    // todo: is this the right destructor? a destructor is anyway neede because if it is destroyed and m_display is not
    // set, a error is raised when trying to delete...
    //if (m_display) {
    //    m_display.reset();
    //}
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
        m_display->draw_rect(x + 13,
                             staff_y + ((m_line - tail_length + 4) * line_height) - staff_to_0,
                             (tail_length + 2 - distance) * line_height - distance, 1);
    } else {
        m_display->draw_rect(x + 4,
                             staff_y + ((m_line + 6) * line_height) - staff_to_0,
                             (tail_length + 2 - distance) * line_height - distance, 1);
    }
}

void Notation::draw_connectors(int x, int staff_y, int line, int length, int number, int tail_length) {
    assert(number > 0);
    bool up = line <= direction_line;
    while (number--) {
        if (up) {
            m_display->draw_rect(x + 13,
                                 staff_y + ((line - tail_length + 4) * line_height) - staff_to_0,
                                 connector_height, length);
            line += 2;
        } else {
            m_display->draw_rect(x + 5,
                                 staff_y + ((line + tail_length + 7) * line_height) - 2 - staff_to_0,
                                 connector_height, length);
            line -= 2;
        }
    }
}

void Notation::draw_ledgers(int x, int staff_y) const {
    if (m_line <= 0) {
        if (m_line <= -6) {
            m_display->draw_text(SymLedger, x,
                                 staff_y + 1 + (-6 * line_height) - staff_to_0);
            if (m_line <= -8) {
                m_display->draw_text(SymLedger, x,
                                     staff_y + 1 + (-8 * line_height) - staff_to_0);
            }
        }
    } else {
        if (m_line >= 6) {
            m_display->draw_text(SymLedger, x,
                                 staff_y + 1 + (6 * line_height) - staff_to_0);
            if (m_line >= 8) {
                m_display->draw_text(SymLedger, x,
                                     staff_y + 1 + (8 * line_height) - staff_to_0);
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

void Notation::draw_connected_notes(int &x, int staff_y, vector<vector<Notation>> notations) {
    assert(notations.size() >= 2);

    // todo: assumes all note are in the same direction.
    int max_height = notations[0][0].get_line();
    int min_height = notations[0][0].get_line();

    /*
     * How many connected rests can there really be beamed together?
     * 1/32 - 6/32 - 1/32
     * rests leave the number of beams like the previous note
     */

    int beams = 0, max_beams = 1;
    for (const auto &group : notations) {
        for (const auto &note : group) {
            if (abs(max_height - direction_line) < abs(note.get_line() - direction_line)) {
                max_height = note.get_line();
            }
            if (abs(min_height - direction_line) > abs(note.get_line() - direction_line)) {
                min_height = note.get_line();
            }
            beams = (-2 - (int) note.get_rounded_length());
            if (max_beams < beams) {
                max_beams = beams;
            }
        }
    }

    //cout << min_height << " " << max_height << endl;
    int line_relation = max(abs(max_height - direction_line - max_beams) - abs(min_height - direction_line), 4) + 3;
    //cout << line_relation << endl;
    //todo: line relation needs to be aware of space for connectors.
    //minimum 1.5 real line difference (1 line from the head end)
    //todo: support dot, which changes also the length of the note...

    int line, tail_length, distance = 0;
    x += merge_padding(notations[0])[0];
    for (const auto &group : notations) {
        distance = (int) ((((double) (group[0].get_length() / minimal_supported_fraction) - 1)) *
                          (minimal_distance + minimal_padding)) + merge_padding(group)[1] + minimal_padding;
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
                    distance += merge_padding(*(&group + 1))[0];
                    draw_connectors(x, staff_y, group[0].get_line(), distance, beams, tail_length);
                } else if (&group == &(notations[notations.size() - 2])) {
                    distance += merge_padding(*(&group + 1))[0];
                    Fraction length_resize = minimal_supported_fraction;
                    if (note.get_rounded_length() <= minimal_supported_fraction) {
                        length_resize = length_resize / Fraction(2, 1);
                    }
                    draw_connectors(x, staff_y, group[0].get_line(),
                                    ceil(static_cast<double>(length_resize / minimal_supported_fraction) * distance),
                                    beams, tail_length);
                } else if (&group == &(notations[notations.size() - 1])) {
                    int tmp_distance = (int) (((
                            (double) (notations[notations.size() - 2][0].get_length() / minimal_supported_fraction) -
                            1)) * (minimal_distance + minimal_padding)) +
                                       merge_padding(notations[notations.size() - 2])[1] + minimal_padding +
                                       merge_padding(group)[0];
                    // beam note size, or half if must smaller...
                    Fraction length_resize = minimal_supported_fraction;
                    if ((*(&group - 1))[0].get_rounded_length() <= minimal_supported_fraction) {
                        length_resize = length_resize / Fraction(2, 1);
                    }
                    draw_connectors(
                            x - floor((static_cast<double>(length_resize / minimal_supported_fraction) * tmp_distance)),
                            staff_y, group[0].get_line(),
                            ceil(static_cast<double> (length_resize / minimal_supported_fraction) * tmp_distance),
                            beams, tail_length);
                }
            }
        }
        x += distance;
    }
}

int Notation::calc_needed_space(const vector<vector<Notation>> &notations) {
    int x = 0;

    for (const auto &group : notations) {
        x += (int) ((((double) (group[0].get_length() / minimal_supported_fraction) - 1)) *
                    (minimal_distance + minimal_padding)) + merge_padding(group)[0] + merge_padding(group)[1] +
             minimal_padding;
    }

    return x;
}

void Notation::draw_individual_notes(int &x, int staff_y, const vector<Notation> &group) {
    // todo: assumes all note are in the same direction.

    // todo: maybe draw_connected_notes can call this function

    x += merge_padding(group)[0];
    for (const auto &note : group) {
        note.display(x, staff_y, true);
    }
    x += (int) ((((double) (group[0].get_length() / minimal_supported_fraction) - 1)) *
                (minimal_distance + minimal_padding)) + merge_padding(group)[1] + minimal_padding;;
}

vector<vector<vector<Notation>>> Notation::split_voices(const vector<vector<Notation>> &notation) {
    vector<vector<Notation>> voice_one;
    vector<Notation> voice_one_group;
    vector<Notation> tmp_one;

    vector<vector<Notation>> voice_two;
    vector<Notation> voice_two_group;
    vector<Notation> tmp_two;

    // todo: need to separate sounds and display individually and together at the same time...

    TimeSignature sig = {4, 4};

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
        voice_one.push_back(tmp_one);
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
        voice_two.push_back(tmp_two);
        voice_two_group.clear();
        tmp_two.clear();
    }

    return {voice_one, voice_two};
}

vector<vector<Notation>> Notation::merge_notation(const vector<vector<Notation>> &notation) {
    vector<vector<Notation>> merged_notation = {notation[0]};

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

    return merged_notation;
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

    return fractions;
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

    return fractions;
}

vector<vector<vector<Notation>>>
Notation::split_notation(const vector<vector<Notation> > &notation, Fraction bar) {
    vector<vector<vector<Notation>>> splitted_notation;
    vector<vector<Notation>> part_notation;
    Fraction length_sum = {0, 1};

    // This function ignores ModDot since it will be decided when generating the notation.
    // Also, this code assumes (which is a reasonable assumption) that every group has the same BasicPlaying.

    for (const auto &group : notation) {
        length_sum += group[0].get_length();
        cout << group[0].get_length() << " " << length_sum << endl;
        part_notation.push_back(group);
        assert(length_sum <= bar);
        if (length_sum == bar) {
            splitted_notation.push_back(part_notation);
            part_notation.clear();
            length_sum = {0, 1};
            cout << "got to length" << endl;
        }
    }

    assert(part_notation.empty());

    return splitted_notation;
}

int
Notation::count_remaining_plays(Fraction offset, Fraction beat, vector<vector<Notation>>::const_iterator notation_it) {
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

vector<vector<vector<Notation>>>
Notation::connect_notation(const vector<vector<Notation>> &notation, Fraction beat) {
    vector<vector<vector<Notation>>> connected_notation;
    vector<vector<Notation>> part_notation;
    Fraction length_sum = {0, 1};

    // This function ignores ModDot since it will be decided when generating the notation.
    // Also, this code assumes (which is a reasonable assumption) that every group has the same BasicPlaying.

    // Beaming needs to be done inside a single beat, only between the first and last played note if there are at least
    // two, also includes the rests between them.
    bool connecting = false;
    for (auto group_it = notation.begin(); group_it != notation.end(); group_it++) {
        const auto &group = *group_it;
        cout << "- " << group[0].get_length() << endl;
        length_sum += group[0].get_length();
        // todo: should if any is playing, although if one is playing than there's no rest...
        if ((group[0].get_playing() == BasePlay) && (group[0].get_length() < beat)) {
            connecting = true;
        }
        cout << group[0].get_length() << " " << length_sum << endl;
        part_notation.push_back(group);
        if ((((group_it + 1 == notation.end()) ? 0 : count_remaining_plays(length_sum, beat, group_it))) == 0) {
            connecting = false;
        }
        if (!connecting) {
            connected_notation.push_back(part_notation);
            part_notation.clear();
        }
        if (length_sum >= beat) {
            length_sum = length_sum % beat;
            cout << "got to length" << endl;
        }
    }

    assert(part_notation.empty());

    return connected_notation;
}

vector<vector<Notation>> Notation::convert_notation(const vector<vector<Notation>> &notation, TimeSignature signature) {
    vector<vector<Notation>> generated_notation;

    Fraction offset(0, 1);

    for (const auto &group : notation) {
        // assumes all the group has the same BasicPlaying and length
        auto fractions = split_fraction(signature, offset, group[0].get_rounded_length());
        BasicPlaying playing = group[0].get_playing();
        cout << group[0].get_rounded_length() << " " << offset << endl;
        for (const auto &fraction : fractions) {
            cout << "* " << fraction << endl;
            if (playing == BasePlay) {
                vector<Notation> tmp;
                for (const auto &note : group) {
                    tmp.emplace_back(BasePlay, note.get_instrument(), fraction, note.get_modifiers());
                }
                generated_notation.push_back(tmp);
                tmp.clear();
            } else {
                // todo: only up for now, will change with two voices support.
                // todo: also no modifiers for rests, check for playing.

                // Add ModDot if possible.
                bool dot = false;
                if (!generated_notation.empty()) {
                    auto note = generated_notation[generated_notation.size() - 1][0];
                    if ((note.get_playing() == BasePlay) &&
                        (find(note.get_modifiers().begin(), note.get_modifiers().end(), ModDot) ==
                         note.get_modifiers().end()) && (note.get_length() / fraction == Fraction(2, 1))) {
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

    return generated_notation;
}

vector<vector<vector<Notation>>>
Notation::generate_notation(const vector<vector<Notation>> &raw_notation, TimeSignature signature) {
    Fraction bar = {signature.first, signature.second};
    Fraction beat = {1, signature.second};

    vector<vector<Notation>> merged_stuff = Notation::merge_notation(raw_notation);

    vector<vector<Notation>> converted_notation = Notation::convert_notation(merged_stuff, signature);
    vector<vector<vector<Notation>>> splitted_notation = Notation::split_notation(converted_notation, bar);

    vector<vector<vector<Notation>>> generated_notation;
    vector<vector<vector<Notation>>> small_connected_notation;

    for (const vector<vector<Notation>> &small_notation : splitted_notation) {
        small_connected_notation = Notation::connect_notation(small_notation, beat);
        generated_notation.insert(generated_notation.end(), small_connected_notation.begin(),
                                  small_connected_notation.end());
    }

    return generated_notation;
}

Padding Notation::create_padding(const vector<Modifier> &modifiers) {
    Padding padding = {0, minimal_distance};

    for (const auto &modifier : modifiers) {
        auto got_padding = modifier_to_padding.find(modifier);
        if (got_padding != modifier_to_padding.end()) {
            padding = merge_padding(padding, got_padding->second);
        }
    }

    return padding;
}

Padding Notation::merge_padding(const Padding &a, const Padding &b) {
    Padding padding = {0, 0};

    for (int i = 0; i < tuple_size<Padding>::value; i++) {
        padding[i] = max(a[i], b[i]);
    }

    return padding;
}

Padding Notation::merge_padding(const vector<Notation> &notes) {
    Padding padding = {0, 0};

    for (const Notation &note : notes) {
        padding = merge_padding(padding, note.get_padding());
    }

    return padding;
}

Fraction Notation::sum_length(const vector<vector<Notation>> &notes) {
    Fraction length = {0, 1};

    for (const auto &note_group : notes) {
        length += note_group[0].get_length();
    }

    return length;
}

void Notation::display_notation(const vector<vector<vector<vector<Notation>>>> &notation, TimeSignature signature) {
    m_display->clear_screen();

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

    int init_x = 50, off_x = init_x, off_y = 100, edge_padding = 20;
    Fraction length = {0, 1};
    Fraction bar = {signature.first, signature.second};
    m_display->draw_base(20, off_y, 3, 4);

    for (const auto &voice : notation) {
        for (const vector<vector<Notation>> &note_groups : voice) {
            // The notes do not stretch over bars, so summing the length will not miss bars.
            length += sum_length(note_groups);

            // move to the next line to avoid displaying over the staff.
            if (off_x + calc_needed_space(note_groups) > 800 - edge_padding) {
                off_x = init_x;
                off_y += 100;
                m_display->draw_base(edge_padding, off_y, 3, 4);
            }

            if (note_groups.size() > 1) {
                Notation::draw_connected_notes(off_x, off_y, note_groups);
            } else {
                Notation::draw_individual_notes(off_x, off_y, note_groups[0]);
            }

            if (!static_cast<bool>(length % bar)) {
                off_x += 10;
                m_display->draw_text("\\", off_x, off_y);
                off_x += 10;
            }
            // printing numbers works great.
        }
    }

    m_display->present();
}

Fraction Notation::get_length() const {
    return ((m_length * 2) - (m_length * Fraction(1, pow(2, count(m_modifiers.begin(), m_modifiers.end(), ModDot)))));
}
