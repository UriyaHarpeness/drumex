#pragma once

#include "Fraction.h"
#include "../Enums.h"
#include "../display/Display.h"

#include <map>
#include <numeric>
#include <cassert>
#include <algorithm>

using namespace std;

class Notation {
public:
    static const map<MusicSymbol, pair<MusicSymbolValues, pair<int, int>>> music_symbols_to_values;

    static const map<int, MusicSymbol> rests_to_music_symbols;

    static const map<Instrument, int> instrument_to_line;

    static const int minimal_distance = 14;

    static const int line_height = 5;

    static const int connector_height = 7;

    static const int staff_to_0 = 2 * line_height;

    static const int direction_line = 2;

    static shared_ptr<Display> m_display;

    Notation(BasicPlaying playing, Instrument instrument, Fraction length, vector<Modifier> modifiers);

    ~Notation();

    void draw_modifiers(int staff_x, int staff_y, int col, int tail_length = 7) const;

    void draw_tail(int staff_x, int staff_y, int col, int tail_length = 7) const;

    static void
    draw_connectors(int staff_x, int staff_y, int line, double col, double length, int number, int tail_length = 7);

    void draw_ledgers(int staff_x, int staff_y, int col) const;

    void draw_head(int staff_x, int staff_y, int col) const;

    void display(int staff_x, int staff_y, int col, int tail_length = 7) const;

    //void display(int staff_x, int staff_y, int line, int col);

    static void draw_connected_notes(int staff_x, int staff_y, int initial_col, vector<vector<Notation>> notations);

    static vector<Notation>
    generate_notation(Action action, Fraction play, Fraction offset, TimeSignature signature, BasicPlaying base);

    [[nodiscard]] inline int get_line() const { return m_line; }

    [[nodiscard]] inline Instrument get_instrument() const { return m_instrument; }

    [[nodiscard]] inline BasicPlaying get_playing() const { return m_playing; }

    [[nodiscard]] Fraction get_length() const;

    [[nodiscard]] inline Fraction get_rounded_length() const { return m_length; }

    [[nodiscard]] inline vector<Modifier> get_modifiers() const { return m_modifiers; }

private:
    MusicSymbol m_symbol;

    pair<MusicSymbolValues, pair<int, int>> m_symbol_value;

    int m_line;

    Fraction m_length;

    Instrument m_instrument;

    BasicPlaying m_playing;

    vector<Modifier> m_modifiers;

    int m_minimal_needed_distance;
};
