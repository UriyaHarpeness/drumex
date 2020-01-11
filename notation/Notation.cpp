#include "Notation.h"

// todo: this is kind of useless now...
const map<MusicSymbol, pair<MusicSymbolValues, pair<int, int>>>
        Notation::music_symbols_to_values = \
        {{WholeRest,        {SymWholeRest,        {0, -10}}},
         {HalfRest,         {SymHalfRest,         {3, -10}}},
         {QuarterRest,      {SymQuarterRest,      {3, -4}}},
         {EightRest,        {SymEightRest,        {3, -4}}},
         {SixteenthRest,    {SymSixteenthRest,    {3, -4}}},
         {ThirtySecondRest, {SymThirtySecondRest, {3, -4}}}};

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
            m_symbol_value = make_pair(SymCymbal, make_pair(3, -10));
        } else {
            if (Fraction(1, 1) == m_length) {
                m_symbol_value = make_pair(SymWholeNote, make_pair(0, -10));
            } else if (Fraction(1, 2) == m_length) {
                m_symbol_value = make_pair(SymHalfNote, make_pair(3, -10));
            } else {
                m_symbol_value = make_pair(SymQuarterNote, make_pair(3, -10));
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
    while (number--) {
        if (line <= direction_line) {
            m_display->draw_rect(x + 13,
                                 staff_y + ((line - tail_length + 4) * line_height) - staff_to_0,
                                 connector_height, length);
        } else {
            //todo: check for all the below line
            m_display->draw_rect(x + 3,
                                 staff_y + ((line + tail_length) * line_height) - staff_to_0,
                                 tail_length * line_height, length);
        }
        line += 2;
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
                         staff_y + (m_line * line_height) - staff_to_0);
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
        for (const auto &note : group) {
            line = note.get_line();
            tail_length = line_relation + (line - min_height);
            note.display(x, staff_y, false, tail_length);
            if (note.get_playing() != BaseRest) {
                // Rests in beams keep the previous note beams number.
                beams = (-2 - (int) note.get_rounded_length());
            }
            distance = (int) ((((double) (note.get_length() / minimal_supported_fraction) - 1)) *
                              (minimal_distance + minimal_padding)) + merge_padding(group)[1] + minimal_padding;
            if (&note == &(group[0])) {
                // is this comparison really valid?
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
                                    ((double) ((note.get_length() - length_resize) / minimal_supported_fraction)) *
                                    distance, beams, tail_length);
                } else if (&group == &(notations[notations.size() - 1])) {
                    // beam note size, or half if must smaller...
                    Fraction length_resize = minimal_supported_fraction;
                    if ((*(&group - 1))[0].get_rounded_length() <= minimal_supported_fraction) {
                        length_resize = length_resize / Fraction(2, 1);
                    }
                    draw_connectors(x - (((double) (((*(&group - 1))[0].get_length() - length_resize) /
                                                    minimal_supported_fraction)) * distance), staff_y,
                                    group[0].get_line(), ((double) (length_resize / minimal_supported_fraction)) *
                                                         distance, beams, tail_length);
                }
            }
        }
        x += distance;
    }
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
    Fraction fill = (fraction <= (beat - (offset % beat))) ? fraction : (beat - (offset % beat));

    tmp = split_fraction(fill);
    fractions.insert(fractions.end(), tmp.begin(), tmp.end());

    offset += fill;
    fraction -= fill;

    while (fraction) {
        fill = (fraction <= (bar - (offset % bar))) ? fraction : (bar - (offset % bar));

        tmp = split_fraction(fill);
        fractions.insert(fractions.end(), tmp.begin(), tmp.end());

        offset += fill;
        fraction -= fill;
    }

    return fractions;
}

vector<vector<Notation>>
Notation::generate_notation(const vector<vector<Notation>> &notation, TimeSignature signature) {
    vector<vector<Notation>> generated_notation;

    Fraction beat(1, signature.second);
    Fraction bar(signature.first, signature.second);
    Fraction offset(0, 1);

    for (const auto &group : notation) {
        // assumes all the group has the same BasicPlaying and length
        auto fractions = split_fraction(signature, offset, group[0].get_rounded_length());
        BasicPlaying playing = group[0].get_playing();
        cout << group[0].get_rounded_length() << " " << offset << endl;
        for (const auto &fraction : fractions) {
            cout << "* " << fraction << endl;
            if (playing == BasePlay) {
                generated_notation.push_back(group);
            } else {
                // todo: only up for now, will change with two voices support.
                generated_notation.push_back({{BaseRest, UnboundUp, fraction, {}}});
            }
            playing = BaseRest;
        }
        offset += group[0].get_rounded_length();
    }

    /*Fraction final_play;
    Fraction final_rest;

    if (!play_space) {
        final_play = beat - offset_space;
    } else {
        if (play_space <= (beat - offset_space)) {
            final_play = play_space;
        } else {
            final_play = beat - offset_space;
        }
    }

    final_rest = play - final_play;

    cout << "play: " << play << ", offset: " << offset << ", signature: " << (int) signature.first << "/"
         << (int) signature.second << endl;

    cout << final_play << " "; // << final_rest << endl;
    // maybe static and pass arguments?
    auto temp = final_rest.split(beat);
    for (auto const &i : temp) {
        cout << i << " ";
    }
    cout << endl;*/
    return generated_notation;


    //if (base == BasePlay) {
    // first play until note value and than rests.
    //}

    /*
     * note of 3/8 (1/4+.)
     * in time signature of 3/4
     * number = 3
     * of = 8
     * note_value = 4
     */
    // todo: will not work with 1/3 on 4/4 for example... need to support tuplets and stuff...
    // plus problematic if a note is as long as several bars, also does not consider other notes that may break the consistency.
    /*for (int i = of; (number > 0) && (i > 0); i /= 2) {
        //cout << number << " " << i << endl;
        if (number >= i) {
            cout << number / i << " - " << i << "/" << of << endl;
            number %= i;
            //gcd(a, b);
        }
    }
    cout << "end " << number << endl;*/

    // in drumming, a played note will not be longer than the basic note value of the signature,
    /*number *= signature.second;
    signature.first *= of;
    of *= signature.second;
    signature.second = of;

    int whole_bars = number / signature.first;
    cout << "using " << whole_bars << " full bars" << endl;

    number %= signature.first;
    cout << "left with " << number << "/" << of << endl;

    double temp = log2(of);
    if (!of || (temp == floor(temp))) {
        cout << "normal note possible" << endl;
    } else {
        cout << "odd tempo needed" << endl;
        // can try fitting anyway to power of 2, like 3/6 to 2/4...
        // 3/6 3/12 5/10
        // leaves the non full notes, which matter
        int note_number = number % of;
        if (!note_number) {
            cout << "writeable with full notes" << endl;
        } else {
            if (of % note_number == 0) {
                temp = log2(of / note_number);
                if (temp == floor(temp)) {
                    cout << "conversion to power of 2 possible" << endl;
                } else {
                    cout << "not convertible" << endl;
                }
            } else {
                cout << "not rational" << endl;
            }
        }
    }
    return notations;

    TimeSignature new_signature = signature;
    new_signature.first *= of;
    new_signature.second *= of;
    int new_number = number * signature.second;
    int new_of = of * signature.second;

    cout << "fitting " << number << "/" << of << " into " << (int) signature.first << "/" << (int) signature.second
         << endl;

    int full_divisions = new_number / new_signature.first;
    cout << "full note values: " << full_divisions << endl;*/

    /*
     * Irrational notes are rounded up for the notes used, for example: 2/3 would contain 2-1/2, 3/7 would contain 3-1/4.
     *
     * Irrational note values are not used when not in a context of other note values, 3/5 is the same as 3/4 if the
     * whole piece keeps the same signature...
     */

    // also support 2 whole, 4 whole, etc.
    /*notations.resize(notations.size() + (full_divisions / signature.first), Notation(WholeNote));
    notations.resize(notations.size() + (full_divisions % signature.first), Notation(BasePlay, signature.second));

    int ratio = (of * gcd(of, signature.second)) / signature.second;

    for (int i = of; (number > 0) && (i > 0); i /= 2) {
        //cout << number << " " << i << endl;
        if (number >= i) {
            cout << number / i << " - " << i << "/" << of << endl;
            number %= i;
            //gcd(a, b);
        }
    }
    cout << "end " << number << endl;*/

    //return notations;
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

Fraction Notation::get_length() const {
    // supports a single dot for now.
    int dot_count = count(m_modifiers.begin(), m_modifiers.end(), ModDot);
    if (!dot_count) {
        return m_length;
    }

    return ((m_length * 2) - (m_length * Fraction(1, pow(2, dot_count))));
}
