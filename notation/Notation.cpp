#include "Notation.h"

// todo: this is kind of useless now...
const map<MusicSymbol, pair<pair<MusicSymbolValues, pair<int, int>>, pair<MusicSymbolValues, pair<int, int>>>>
        Notation::music_symbols_to_values = \
        {{WholeNote,   {{SymWholeNote,     {0, -10}}, {SymWholeNote,       {0, -10}}}},
         {HalfNote,    {{SymUpHalfNote,    {3, -10}}, {SymDownHalfNote,    {3, -10}}}},
         {QuarterNote, {{SymUpQuarterNote, {3, -4}},  {SymDownQuarterNote, {3, -4}}}},
         {EightNote,   {{SymUpEightNote,   {3, -4}},  {SymDownEightNote,   {3, -4}}}}};

// further numbers would be generated, the values should stay as number or this will be endless...
const map<BasicPlaying, map<int, MusicSymbol>> Notation::playing_to_music_symbols = \
{{BasePlay, {{0, WholeNote}, {-1, HalfNote}, {-2, QuarterNote}, {-3, EightNote}, {-4, SixteenthNote}}},
 {BaseStop, {{0, WholeStop}, {-1, HalfStop}, {-2, QuarterStop}, {-3, EightStop}, {-4, SixteenthNote}}}};

const map<Instrument, int> Notation::instrument_to_line = {
        {ChinaInst,   -8},
        {HiHatInst,   -5},
        {HighTomInst, -3},
        {SnareInst,   -1},
        {BassInst,    3}
};

shared_ptr<Display> Notation::m_display = nullptr;

/*Notation::Notation(MusicSymbol symbol) : m_symbol(symbol) {

}*/

Notation::Notation(BasicPlaying playing, Instrument instrument, Fraction length, vector<Modifier> modifiers) :
        m_length(length), m_instrument(instrument), m_playing(playing), m_modifiers(modifiers) {
    // todo: support 2 whole, etc.
    //m_symbol = playing_to_music_symbols.at(playing).at(length);
}

Notation::~Notation() {
    // todo: is this the right destructor? a destructor is anyway neede because if it is destroyed and m_display is not
    // set, a error is raised when trying to delete...
    //if (m_display) {
    //    m_display.reset();
    //}
}

void Notation::draw_tail(int staff_x, int staff_y, int col, int tail_length) const {
    int line = instrument_to_line.at(m_instrument);
    if (line <= direction_line) {
        m_display->draw_rect(staff_x + (col * minimal_distance) + 13,
                             staff_y + ((line - tail_length + 4) * line_height) - staff_to_0,
                             (tail_length + 2) * line_height, 1);
    } else {
        m_display->draw_rect(staff_x + (col * minimal_distance) + 3,
                             staff_y + ((line + tail_length) * line_height) - staff_to_0,
                             tail_length * line_height, 1);
    }
}

void Notation::draw_connectors(int staff_x, int staff_y, int line, int col, int length, int number, int tail_length) {
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
    int line = instrument_to_line.at(m_instrument);
    if (line <= 0) {
        if (line <= -6) {
            m_display->draw_text(SymLedger, staff_x + (col * minimal_distance),
                                 staff_y + 1 + (-6 * line_height) - staff_to_0);
            if (line <= -8) {
                m_display->draw_text(SymLedger, staff_x + (col * minimal_distance),
                                     staff_y + 1 + (-8 * line_height) - staff_to_0);
            }
        }
    } else {
        if (line >= 6) {
            m_display->draw_text(SymLedger, staff_x + (col * minimal_distance),
                                 staff_y + 1 + (6 * line_height) - staff_to_0);
            if (line >= 8) {
                m_display->draw_text(SymLedger, staff_x + (col * minimal_distance),
                                     staff_y + 1 + (8 * line_height) - staff_to_0);
            }
        }
    }
}

void Notation::draw_head(int staff_x, int staff_y, int col) const {
    int line = instrument_to_line.at(m_instrument);
    MusicSymbol symbol;
    pair<MusicSymbolValues, pair<int, int>> symbol_value;
    if (m_playing == BasePlay) {
        //todo: no more than whole note support for now.
        //todo: are music symbol and music symbols values needed?
        if (Fraction(1, 1) == m_length) {
            symbol_value = make_pair(SymWholeNote, make_pair(0, -10));
        } else if (Fraction(1, 2) == m_length) {
            symbol_value = make_pair(SymHalfNote, make_pair(3, -10));
        } else {
            symbol_value = make_pair(SymQuarterNote, make_pair(3, -10));
        }
    } else {
        symbol = playing_to_music_symbols.at(m_playing).at(m_length);
        symbol_value = music_symbols_to_values.at(symbol).first;
    }
    m_display->draw_text(symbol_value.first, staff_x + symbol_value.second.first + (col * minimal_distance),
                         staff_y + (line * line_height) - staff_to_0);
}

void Notation::display(int staff_x, int staff_y, int col, int tail_length) const {
    draw_ledgers(staff_x, staff_y, col);
    if (m_length < Fraction(1, 1)) {
        draw_tail(staff_x, staff_y, col, tail_length);
    }
    draw_head(staff_x, staff_y, col);
}

void Notation::draw_connected_notes(int staff_x, int staff_y, int initial_col, vector<vector<Notation>> notations) {
    // todo: assumes all note are in the same direction.
    int max_height = instrument_to_line.at(notations[0][0].get_instrument());
    int min_height = instrument_to_line.at(notations[0][0].get_instrument());

    int beams, max_beams = 1;
    for (const auto &group : notations) {
        for (const auto &note : group) {
            if (abs(max_height - direction_line) < abs(instrument_to_line.at(note.get_instrument()) - direction_line)) {
                max_height = instrument_to_line.at(note.get_instrument());
            }
            if (abs(min_height - direction_line) > abs(instrument_to_line.at(note.get_instrument()) - direction_line)) {
                min_height = instrument_to_line.at(note.get_instrument());
            }
            beams = (-2 - (int) note.get_length());
            if (max_beams < beams) {
                max_beams = beams;
            }
        }
    }

    //cout << min_height << " " << max_height << endl;
    int line_relation = max(abs(max_height - direction_line) - abs(min_height - direction_line), 4) + 3;
    //cout << line_relation << endl;
    //todo: line relation needs to be aware of space for connectors.
    //minimum 1.5 real line difference (1 line from the head end)

    int line, tail_length;
    for (const auto &group : notations) {
        for (const auto &note : group) {
            line = instrument_to_line.at(note.get_instrument());
            tail_length = line_relation + (line - min_height);
            note.display(staff_x, staff_y, initial_col, tail_length);
            beams = (-2 - (int) note.get_length());
            if ((&note == &(group[0])) && (&group != &(notations[notations.size() - 1]))) {
                draw_connectors(staff_x, staff_y, instrument_to_line.at(group[0].get_instrument()), initial_col,
                                (int) ((double) (note.get_length() / Fraction(1, 16))), beams, tail_length);
            }
        }
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
    Fraction final_stop;

    if (!play_space) {
        final_play = beat - offset_space;
    } else {
        if (play_space <= (beat - offset_space)) {
            final_play = play_space;
        } else {
            final_play = beat - offset_space;
        }
    }

    final_stop = play - final_play;

    cout << play << " " << offset << " " << (int) signature.first << "/" << (int) signature.second << endl;

    cout << final_play << " "; // << final_stop << endl;
    // maybe static and pass arguments?
    auto temp = final_stop.split(beat);
    for (auto const &i : temp) {
        cout << i << " ";
    }
    cout << endl;
    return notations;


    if (base == BasePlay) {
        // first play until note value and than stops.
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
