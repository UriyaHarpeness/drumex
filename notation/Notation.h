#pragma once

#include "Fraction.h"
#include "../Enums.h"
#include "../display/Display.h"
#include "../metronome/Metronome.h"

#include <map>
#include <numeric>
#include <cassert>
#include <algorithm>

using namespace std;

class Notation;

typedef array<int, 2> Padding;
typedef vector<pair<Fraction, Padding>> Paddings;
typedef vector<Notation> Group;
typedef vector<Group> Voice;
typedef vector<Voice> Notations;
typedef vector<Notations> GroupedNotations;

class Notation {
public:
    static const map<MusicSymbol, pair<MusicSymbolValues, pair<int, int>>> music_symbols_to_values;

    static const map<int, MusicSymbol> rests_to_music_symbols;

    static const map<Instrument, int> instrument_to_line;

    static const map<Modifier, Padding> modifier_to_padding;

    static const map<Modifier, MusicSymbolValues> modifier_to_symbol;

    static const map<Modifier, array<int, 3>> modifier_to_position;

    static const int minimal_distance = 14;

    static const int line_height = 5;

    static const int connector_height = 7;

    static const int staff_to_0 = 2 * line_height;

    static const int direction_line = 2;

    static const int displaying_init_x = 50;

    static const int displaying_init_y = 100;

    static const int staff_lines_spacing = 100;

    static const int max_lines_displayed = 4;

    static const Fraction minimal_supported_fraction;

    static shared_ptr<Display> m_display;

    Notation(const Notation &other);

    Notation(BasicPlaying playing, Instrument instrument, const Fraction &length, const vector<Modifier> &modifiers);

    ~Notation();

    void draw_modifiers(int x, int staff_y, int tail_length = 7) const;

    void draw_flags(int x, int staff_y, int tail_length = 7) const;

    void draw_tail(int x, int staff_y, int tail_length = 7) const;

    static void draw_connectors(int x, int staff_y, int line, int length, int number, int tail_length = 7);

    void draw_ledgers(int x, int staff_y) const;

    void draw_head(int x, int staff_y) const;

    void display(int x, int staff_y, bool flags = true, int tail_length = 7) const;

    static int calc_needed_space(const Paddings &distances, Fraction offset, Fraction length);

    static void
    draw_connected_notes(int &x, int staff_y, const Paddings &distances, Fraction offset, const Voice &notations);

    static void
    draw_individual_notes(int &x, int staff_y, const Paddings &distances, const Fraction &offset, const Group &group);

    static Notations split_voices(const Voice &notation);

    /**
     * Unify notation where possible, optimize the notation and remove replaceable clutter.
     *
     * @example 1/4 play + 1/4 rest = 1/2 play.
     * @param notation  Raw notation with plays and rests possibly.
     * @return  The merged notation.
     */
    static Voice merge_notation(const Voice &notation);

    static Fraction merge_lengths(const vector<Fraction> &lengths);

    static vector<Fraction> split_fraction(Fraction fraction);

    static vector<Fraction> split_fraction(TimeSignature signature, Fraction offset, Fraction fraction);

    static Notations split_notation(const Voice &notation, const Fraction &bar);

    static int count_remaining_plays(Fraction offset, const Fraction &beat, Voice::const_iterator notation_it);

    static Notations connect_notation(const Voice &notation, const Fraction &beat);

    static Voice convert_notation(const Voice &notation, TimeSignature signature);

    static Voice generate_voice_notation(const Voice &raw_voice_notation, TimeSignature signature);

    static Notations generate_notation(const Voice &raw_notation, TimeSignature signature);

    static void stretch_notation(Notations &notation, Fraction old_length, Fraction new_length);

    static Padding create_padding(const vector<Modifier> &modifiers);

    static void insert_padding(Paddings &paddings, const Fraction &offset, Padding padding);

    static Padding merge_padding(const Padding &a, const Padding &b);

    static Padding merge_padding(const Group &notes);

    static Fraction sum_length(const Voice &notes);

    static Fraction sum_length(const Notations &notes);

    static Fraction sum_final_length(const Notations &notes);

    /**
     * Display the notation.
     *
     * @param notation  The notation do display, its type is complicated and here's why, it's a vector:
     *                  1.  of single notes.
     *                  2.  of grouped notes (played at the same time with same length).
     *                  3.  of connected notes (notes that are beamed tigether).
     *                  4.  of voices.
     * @param signature The time signature.
     */
    static void display_notation(const vector<vector<vector<vector<Notation>>>> &notation,
                                 const Paddings &distances, const Fraction &bar,
                                 const int played_line);

    static pair<pair<int, int>, Padding> get_note_location(const GroupedNotations &notation, const Paddings &distances,
                                                           const Fraction &bar, const Fraction &location);

    static void
    continuous_display_notation(const GroupedNotations &notation, const Paddings &distances, const Fraction &bar,
                                int tempo);

    static void
    prepare_displayable_notation(const Notations &generated_notation, GroupedNotations &notation, Paddings &distances,
                                 const Fraction &bar);

    [[nodiscard]] inline int get_line() const { return m_line; }

    [[nodiscard]] inline Instrument get_instrument() const { return m_instrument; }

    [[nodiscard]] inline BasicPlaying get_playing() const { return m_playing; }

    [[nodiscard]] Fraction get_length() const;

    [[nodiscard]] inline Fraction get_rounded_length() const { return m_length; }

    inline void set_rounded_length(const Fraction &length) { m_length = length; }

    void reset_length(const Fraction &length);

    [[nodiscard]] inline vector<Modifier> get_modifiers() const { return m_modifiers; }

    inline void add_modifier(Modifier modifier) { m_modifiers.push_back(modifier); }

    [[nodiscard]] inline Padding get_padding() const { return m_padding; }

    [[nodiscard]] static inline Padding get_distance(const Fraction &length, Padding padding) {
        return {padding[0],
                max(static_cast<int>(static_cast<double>(length / minimal_supported_fraction) * minimal_distance),
                    minimal_distance + padding[1])};
    }

private:
    MusicSymbol m_symbol;

    pair<MusicSymbolValues, pair<int, int>> m_symbol_value;

    int m_line;

    Fraction m_length;

    Instrument m_instrument;

    BasicPlaying m_playing;

    vector<Modifier> m_modifiers;

    Padding m_padding;
};
