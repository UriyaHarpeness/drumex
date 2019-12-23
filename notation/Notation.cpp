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
        {ChinaInst,   -8},
        {HiHatInst,   -5},
        {HighTomInst, -3},
        {SnareInst,   -1},
        {BassInst,    3},
        {UnboundUp,   -4},
        {UnboundDown, 4}
};

// todo: change to 1/32 after dynamic resizing.
const Fraction Notation::minimal_supported_fraction(1, 16);

shared_ptr<Display> Notation::m_display = nullptr;

Notation::Notation(BasicPlaying playing, Instrument instrument, Fraction length, vector<Modifier> modifiers) :
        m_line(instrument_to_line.at(instrument)), m_length(length), m_instrument(instrument), m_playing(playing),
        m_modifiers(modifiers) {
    // todo: support 2 whole, etc.
    //m_symbol = playing_to_music_symbols.at(playing).at(length);
    if (m_playing == BasePlay) {
        //todo: no more than whole note support for now.
        //todo: are music symbol and music symbols values needed?

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

void Notation::draw_modifiers(int staff_x, int staff_y, int col, int tail_length) const {
    // currently supports one dot, also may not support more dots in future since more than one makes the notation
    // confusing.
    if (find(m_modifiers.begin(), m_modifiers.end(), ModDot) != m_modifiers.end()) {
        // todo: problematic with notes that have the head pointing the other direction.
        m_display->draw_text(SymDot, staff_x, staff_y, col, m_line, 16, -4);
    }
    if (find(m_modifiers.begin(), m_modifiers.end(), ModAccent) != m_modifiers.end()) {
        // todo: also problematic when one connected notes have this only, plus others.
        // also, supports only upper notes for now.
        m_display->draw_text(SymAccent, staff_x, staff_y, col, m_line, 0, -(tail_length + 4) * line_height - 2);
    }
}

void Notation::draw_tail(int staff_x, int staff_y, int col, int tail_length) const {
    int distance = (m_symbol_value.first == SymCymbal) ? 1 : 0;
    if (m_line <= direction_line) {
        m_display->draw_rect(staff_x + (col * minimal_distance) + 13,
                             staff_y + ((m_line - tail_length + 4) * line_height) - staff_to_0,
                             (tail_length + 2 - distance) * line_height - distance, 1);
    } else {
        //todo: this real
        m_display->draw_rect(staff_x + (col * minimal_distance) + 3,
                             staff_y + ((m_line + tail_length) * line_height) - staff_to_0,
                             (tail_length) * line_height, 1);
    }
}

void
Notation::draw_connectors(int staff_x, int staff_y, int line, double col, double length, int number, int tail_length) {
    while (number--) {
        if (line <= direction_line) {
            m_display->draw_rect(staff_x + (col * minimal_distance) + 13,
                                 staff_y + ((line - tail_length + 4) * line_height) - staff_to_0,
                                 connector_height, length * minimal_distance);
        } else {
            //todo: check for all the below line
            m_display->draw_rect(staff_x + (col * minimal_distance) + 3,
                                 staff_y + ((line + tail_length) * line_height) - staff_to_0,
                                 tail_length * line_height, 1);
        }
        line += 2;
    }
}

void Notation::draw_ledgers(int staff_x, int staff_y, int col) const {
    if (m_line <= 0) {
        if (m_line <= -6) {
            m_display->draw_text(SymLedger, staff_x + (col * minimal_distance),
                                 staff_y + 1 + (-6 * line_height) - staff_to_0);
            if (m_line <= -8) {
                m_display->draw_text(SymLedger, staff_x + (col * minimal_distance),
                                     staff_y + 1 + (-8 * line_height) - staff_to_0);
            }
        }
    } else {
        if (m_line >= 6) {
            m_display->draw_text(SymLedger, staff_x + (col * minimal_distance),
                                 staff_y + 1 + (6 * line_height) - staff_to_0);
            if (m_line >= 8) {
                m_display->draw_text(SymLedger, staff_x + (col * minimal_distance),
                                     staff_y + 1 + (8 * line_height) - staff_to_0);
            }
        }
    }
}

void Notation::draw_head(int staff_x, int staff_y, int col) const {
    m_display->draw_text(m_symbol_value.first, staff_x + m_symbol_value.second.first + (col * minimal_distance),
                         staff_y + (m_line * line_height) - staff_to_0);
}

void Notation::display(int staff_x, int staff_y, int col, int tail_length) const {
    draw_ledgers(staff_x, staff_y, col);
    draw_modifiers(staff_x, staff_y, col, tail_length);
    if ((m_length < Fraction(1, 1)) && (m_playing != BaseRest)) {
        draw_tail(staff_x, staff_y, col, tail_length);
    }
    draw_head(staff_x, staff_y, col);
}

void Notation::draw_connected_notes(int staff_x, int staff_y, int initial_col_i, vector<vector<Notation>> notations) {
    assert(notations.size() >= 2);

    // todo: assumes all note are in the same direction.
    int max_height = notations[0][0].get_line();
    int min_height = notations[0][0].get_line();

    double initial_col = initial_col_i;

    /*
     * How many connected rests can there really be beamed together?
     * 1/32 - 6/32 - 1/32
     * rests leave the number of beams like the previous note
     */

    int beams, max_beams = 1;
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

    int line, tail_length;
    for (const auto &group : notations) {
        for (const auto &note : group) {
            line = note.get_line();
            tail_length = line_relation + (line - min_height);
            note.display(staff_x, staff_y, initial_col, tail_length);
            if (note.get_playing() != BaseRest) {
                // Rests in beams keep the previous note beams number.
                beams = (-2 - (int) note.get_rounded_length());
            }
            if (&note == &(group[0])) {
                // is this comparison really valid?
                if (&group < &(notations[notations.size() - 2])) {
                    draw_connectors(staff_x, staff_y, group[0].get_line(), initial_col,
                                    ((double) (note.get_length() / Fraction(1, 16))), beams, tail_length);
                } else if (&group == &(notations[notations.size() - 2])) {
                    Fraction length_resize = minimal_supported_fraction;
                    if (note.get_rounded_length() <= minimal_supported_fraction) {
                        length_resize = length_resize / Fraction(2, 1);
                    }
                    draw_connectors(staff_x, staff_y, group[0].get_line(), initial_col,
                                    ((double) ((note.get_length() - length_resize) / minimal_supported_fraction)),
                                    beams, tail_length);
                } else if (&group == &(notations[notations.size() - 1])) {
                    // beam note size, or half if must smaller...
                    Fraction length_resize = minimal_supported_fraction;
                    if ((*(&group - 1))[0].get_rounded_length() <= minimal_supported_fraction) {
                        length_resize = length_resize / Fraction(2, 1);
                    }
                    draw_connectors(staff_x, staff_y, group[0].get_line(),
                                    initial_col - ((double) (((*(&group - 1))[0].get_length() - length_resize) /
                                                             minimal_supported_fraction)),
                                    (double) (length_resize / minimal_supported_fraction),
                                    beams, tail_length);
                }
            }
        }
        //double d = ((double) (group[0].get_length() / Fraction(1, 16)));
        //cout << group[0].get_length() << endl;
        //cout << (group[0].get_length() / Fraction(1, 16)) << endl;
        //cout << d << endl;
        initial_col += (int) ((double) (group[0].get_length() / Fraction(1, 16)));
    }
}

vector<Notation>
Notation::generate_notation(Action action, Fraction play, Fraction offset, TimeSignature signature, BasicPlaying base) {
    vector<Notation> notations;

    if (!play) {
        return notations;
    }

    Fraction beat(1, signature.second);
    Fraction offset_space = offset % beat;
    Fraction play_space = (play < beat) ? play : beat;
    Fraction final_play;
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

    cout << play << " " << offset << " " << (int) signature.first << "/" << (int) signature.second << endl;

    cout << final_play << " "; // << final_rest << endl;
    // maybe static and pass arguments?
    auto temp = final_rest.split(beat);
    for (auto const &i : temp) {
        cout << i << " ";
    }
    cout << endl;
    return notations;


    if (base == BasePlay) {
        // first play until note value and than rests.
    }

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

    return notations;
}

Fraction Notation::get_length() const {
    // supports a single dot for now.
    int dot_count = count(m_modifiers.begin(), m_modifiers.end(), ModDot);
    if (!dot_count) {
        return m_length;
    }

    return ((m_length * 2) - (m_length * Fraction(1, pow(2, dot_count))));
}
