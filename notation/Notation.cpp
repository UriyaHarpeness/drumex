#include "Notation.h"

const map<MusicSymbol, pair<pair<MusicSymbolValues, pair<int, int>>, pair<MusicSymbolValues, pair<int, int>>>>
        Notation::music_symbols_to_values = \
        {{WholeNote,   {{SymWholeNote,     {0, -10}}, {SymWholeNote,       {0, -10}}}},
         {HalfNote,    {{SymUpHalfNote,    {3, -15}}, {SymDownHalfNote,    {3, -4}}}},
         {QuarterNote, {{SymUpQuarterNote, {3, -15}}, {SymDownQuarterNote, {3, -4}}}},
         {EightNote,   {{SymUpEightNote,   {3, -15}}, {SymDownEightNote,   {3, -4}}}}};

// further numbers would be generated, the values should stay as number or this will be endless...
const map<BasicPlaying, map<int, MusicSymbol>> Notation::playing_to_music_symbols = \
{{BasePlay, {{1, WholeNote}, {2, HalfNote}, {4, QuarterNote}, {8, EightNote}}},
 {BaseStop, {{1, WholeStop}, {2, HalfStop}, {4, QuarterStop}, {8, EightStop}}}};

shared_ptr<Display> Notation::m_display = nullptr;

Notation::Notation(MusicSymbol symbol) : m_symbol(symbol) {

}

Notation::Notation(BasicPlaying playing, int length) {
    // todo: support 2 whole, etc.
    m_symbol = playing_to_music_symbols.at(playing).at(length);
}

Notation::~Notation() {
    // todo: is this the right destructor? a destructor is anyway neede because if it is destroyed and m_display is not
    // set, a error is raised when trying to delete...
    if (m_display) {
        m_display.reset();
    }
}

void Notation::display(int staff_x, int staff_y, int line, int col) {
    if (line <= 0) {
        auto symbol_value = music_symbols_to_values.at(m_symbol).first;
        if (line <= -6) {
            m_display->draw_text(SymLedger, staff_x + (col * 32), staff_y + 1 - 40);
            if (line <= -8) {
                m_display->draw_text(SymLedger, staff_x + (col * 32), staff_y + 1 - 50);
            }
        }
        m_display->draw_text(symbol_value.first, staff_x + symbol_value.second.first + (col * 32),
                             staff_y + symbol_value.second.second + (line * 5));
    } else {
        auto symbol_value = music_symbols_to_values.at(m_symbol).second;
        m_display->draw_text(symbol_value.first, staff_x + symbol_value.second.first + (col * 32),
                             staff_y + symbol_value.second.second + (line * 5));
        if (line >= 6) {
            m_display->draw_text(SymLedger, staff_x + (col * 32), staff_y + 1 + 20);
            if (line >= 8) {
                m_display->draw_text(SymLedger, staff_x + (col * 32), staff_y + 1 + 30);
            }
        }
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
