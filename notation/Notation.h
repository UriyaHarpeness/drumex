#pragma once

#include "Fraction.h"
#include "../Enums.h"
#include "../display/Display.h"

#include <map>
#include <numeric>
#include <cassert>
#include <algorithm>

using namespace std;

class Notation;

typedef array<int, 2> Padding;
typedef map<Fraction, Padding> Paddings;
// A mapping of absolute offset to the location and its padding.
typedef map<Fraction, pair<int, Padding>> GlobalLocations;

typedef vector<Notation> Group;
typedef vector<Group> Voice;
typedef vector<Voice> BeamedVoice;
typedef vector<Voice> Notations;
typedef vector<Notations> GroupedNotations;

class Notation {
public:
    static const map<MusicSymbols, pair<int, int>> music_symbols_to_values;

    static const map<int, MusicSymbols> rests_to_music_symbols;

    static const map<Instrument, int> instrument_to_line;

    static const map<Modifier, Padding> modifier_to_padding;

    static const map<Modifier, MusicSymbols> modifier_to_symbol;

    static const map<Modifier, array<int, 3>> modifier_to_position;

    static const int minimal_distance = 14;

    static const int line_height = 5;

    static const int staff_to_0 = 2 * line_height;

    static const int direction_line = 2;

    static shared_ptr<Display> m_display;

    Notation(const Notation &other);

    Notation(BasicPlaying playing, Instrument instrument, const Fraction &length, const vector<Modifier> &modifiers);

    ~Notation();

    void initialize_with_length();

    void draw_modifiers(int x, int staff_y, int tail_length = 7) const;

    void draw_flags(int x, int staff_y, int tail_length = 7) const;

    void draw_tail(int x, int staff_y, int tail_length = 7) const;

    void draw_ledgers(int x, int staff_y) const;

    void draw_head(int x, int staff_y) const;

    void display(int x, int staff_y, bool flags = true, int tail_length = 7) const;

    static Padding create_padding(const vector<Modifier> &modifiers);

    static Padding merge_padding(const Padding &a, const Padding &b);

    [[nodiscard]] inline int get_line() const { return m_line; }

    [[nodiscard]] inline Instrument get_instrument() const { return m_instrument; }

    [[nodiscard]] inline BasicPlaying get_playing() const { return m_playing; }

    /**
     * Get the length, takes into account the basic length and dots. Does not take ratio into account.
     *
     * @return  The length.
     */
    [[nodiscard]] Fraction get_length() const;

    /**
     * Get the rounded length, takes into account only the basic length. Does not take ratio and dots into account.
     *
     * @return  The rounded length.
     */
    [[nodiscard]] inline Fraction get_rounded_length() const { return m_length; }

    [[nodiscard]] inline Fraction get_simple_length() const {
        return Fraction(m_length.get_value().first, pow(2, (int) log2(m_length.get_value().second)));
    }

    inline void set_rounded_length(const Fraction &length) { m_length = length; }

    void reset_length(const Fraction &length);

    [[nodiscard]] inline vector<Modifier> get_modifiers() const { return m_modifiers; }

    inline void add_modifier(Modifier modifier) { m_modifiers.push_back(modifier); }

    [[nodiscard]] inline Padding get_padding() const { return m_padding; }

private:
    MusicSymbols m_symbol;

    pair<int, int> m_symbol_value;

    int m_line{};

    Fraction m_length;

    Instrument m_instrument;

    BasicPlaying m_playing;

    vector<Modifier> m_modifiers{};

    Padding m_padding{};
};
