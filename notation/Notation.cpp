#include "Notation.h"

const map<MusicSymbols, pair<int, int>> Notation::music_symbols_to_values = {
        {SymWholeRest,        {3, 5}},
        {SymHalfRest,         {3, 5}},
        {SymQuarterRest,      {3, 0}},
        {SymEightRest,        {3, 0}},
        {SymSixteenthRest,    {3, 0}},
        {SymThirtySecondRest, {3, 0}},
        {SymSixtyFourthRest,  {3, 0}}
};

// further numbers would be generated, the values should stay as number or this will be endless...
const map<int, MusicSymbols> Notation::rests_to_music_symbols = {
        {0,  SymWholeRest},
        {-1, SymHalfRest},
        {-2, SymQuarterRest},
        {-3, SymEightRest},
        {-4, SymSixteenthRest},
        {-5, SymThirtySecondRest},
        {-6, SymSixtyFourthRest}
};

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

const map<Modifier, MusicSymbols> Notation::modifier_to_symbol = {
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

shared_ptr<Display> Notation::m_display = nullptr;

Notation::Notation(const Notation &other) = default;

Notation::~Notation() = default;

Notation::Notation(BasicPlaying playing, Instrument instrument, const Fraction &length,
                   const vector<Modifier> &modifiers) :
        m_line(instrument_to_line.at(instrument)), m_length(length), m_instrument(instrument), m_playing(playing),
        m_modifiers(modifiers), m_padding(create_padding(modifiers)) {
    // todo: support 2 whole, etc, no more than whole note support for now.
    initialize_with_length();
}

void Notation::initialize_with_length() {
    if (m_playing == BasePlay) {
        // todo: support flam and drag that are not tied to the next note hit (like quick flam of snare and bass)
        if (m_line <= -4) {
            // todo: there are more places with cymbals, like hihat foot.
            if (Fraction(1, 2) <= get_simple_length()) {
                m_symbol = SymWholeCymbal;
                m_symbol_value = make_pair(3, 0);
            } else {
                m_symbol = SymQuarterCymbal;
                m_symbol_value = make_pair(3, 0);
            }
        } else {
            if (Fraction(1, 1) == get_simple_length()) {
                m_symbol = SymWholeNote;
                m_symbol_value = make_pair(0, 0);
            } else if (Fraction(1, 2) == get_simple_length()) {
                m_symbol = SymHalfNote;
                m_symbol_value = make_pair(3, 0);
            } else {
                m_symbol = SymQuarterNote;
                m_symbol_value = make_pair(3, 0);
            }
        }
    } else {
        // Before generating the standardized notation, this case may be possible.
        if (rests_to_music_symbols.find(static_cast<int>(get_simple_length())) != rests_to_music_symbols.end()) {
            m_symbol = rests_to_music_symbols.at(static_cast<int>(get_simple_length()));
            m_symbol_value = music_symbols_to_values.at(m_symbol);
        }
    }
}

void Notation::draw_modifiers(int x, int staff_y, int tail_length) const {
    // currently supports one dot, also may not support more dots in future since more than one makes the notation
    // confusing.

    for (const Modifier &modifier: m_modifiers) {
        const auto position = modifier_to_position.at(modifier);
        // todo: also problematic when one connected notes have this only, plus others.
        if (m_line <= DisplayConstants::direction_line) {
            m_display->draw_text(modifier_to_symbol.at(modifier), x, staff_y, m_line, position[0],
                                 ((position[1] ? (-(tail_length + position[1]) * DisplayConstants::line_height) : 0) +
                                  position[2]));
        } else {
            m_display->draw_text(modifier_to_symbol.at(modifier), x, staff_y, m_line, position[0],
                                 ((position[1] ? (-(-1 + position[1]) * DisplayConstants::line_height) : 0) +
                                  position[2]));
        }
    }
}

void Notation::draw_flags(int x, int staff_y, int tail_length) const {
    for (int i = 0; i < (-static_cast<int>(get_simple_length())) - 2; i++) {
        if (m_line <= DisplayConstants::direction_line) {
            m_display->draw_text(SymUpFlag, x + 13,
                                 staff_y + ((m_line - tail_length + (i * 2)) * DisplayConstants::line_height) -
                                 DisplayConstants::staff_to_0);

        } else {
            m_display->draw_text(SymDownFlag, x + 4,
                                 staff_y + ((m_line + tail_length - (i * 2)) * DisplayConstants::line_height) -
                                 DisplayConstants::staff_to_0);
        }
    }
}

void Notation::draw_tail(int x, int staff_y, int tail_length) const {
    int distance = ((m_symbol == SymQuarterCymbal) || (m_symbol == SymWholeCymbal)) ? 1 : 0;
    if (m_line <= DisplayConstants::direction_line) {
        m_display->draw_rect(x + 13, staff_y + ((m_line - tail_length + 4) * DisplayConstants::line_height) -
                                     DisplayConstants::staff_to_0,
                             (tail_length + 2 - distance) * DisplayConstants::line_height - distance, 1);
    } else {
        m_display->draw_rect(x + 4,
                             staff_y + ((m_line + 6) * DisplayConstants::line_height) - DisplayConstants::staff_to_0,
                             (tail_length + 2 - distance) * DisplayConstants::line_height - distance, 1);
    }
}

void Notation::draw_ledgers(int x, int staff_y) const {
    for (const auto &it: DisplayConstants::ledger_lines) {
        if (!((m_line > 0) ^ (it > 0)) && (abs(m_line) >= abs(it))) {
            m_display->draw_text(SymLedger, x,
                                 staff_y + 1 + (it * DisplayConstants::line_height) - DisplayConstants::staff_to_0);
        }
    }
}

void Notation::draw_head(int x, int staff_y) const {
    m_display->draw_text(m_symbol, x + m_symbol_value.first,
                         staff_y + (m_line * DisplayConstants::line_height) + m_symbol_value.second -
                         DisplayConstants::staff_to_0);
}

void Notation::display(int x, int staff_y, bool flags, int tail_length) const {
    draw_ledgers(x, staff_y);
    if ((get_simple_length() < Fraction(1, 1)) && (m_playing != BaseRest)) {
        if (flags) {
            draw_flags(x, staff_y, tail_length);
        }
        draw_tail(x, staff_y, tail_length);
    }
    draw_modifiers(x, staff_y, tail_length);
    draw_head(x, staff_y);
}

Padding Notation::create_padding(const vector<Modifier> &modifiers) {
    Padding padding = {0, 0};

    for (const auto &modifier: modifiers) {
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

Fraction Notation::get_length() const {
    return ((m_length * 2) -
            (m_length * Fraction(1, (int) pow(2, count(m_modifiers.begin(), m_modifiers.end(), ModDot)))));
}

void Notation::reset_length(const Fraction &length) {
    m_length = length;
    initialize_with_length();

    auto location = find(m_modifiers.begin(), m_modifiers.end(), ModDot);
    if (location != m_modifiers.end()) {
        m_modifiers.erase(location);
    }
}

ostream &operator<<(ostream &os, const Notation &notation) {
    os << "<Notation> " << find_by_value(playing_names, notation.m_playing) << " " << notation.m_length << " "
       << find_by_value(instrument_names, notation.m_instrument) << " ";
    for (const auto &modifier: notation.m_modifiers) {
        os << find_by_value(modifier_names, modifier) << " ";
    }
    return os;
}
