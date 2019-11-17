#pragma once

#include "Fraction.h"
#include "../Enums.h"
#include "../display/Display.h"

#include <map>
#include <numeric>

using namespace std;

class Notation {
public:
    static const map<MusicSymbol, pair<pair<MusicSymbolValues, pair<int, int>>, pair<MusicSymbolValues, pair<int, int>>>> music_symbols_to_values;

    static const map<BasicPlaying, map<int, MusicSymbol>> playing_to_music_symbols;

    static shared_ptr<Display> m_display;

    explicit Notation(MusicSymbol symbol);

    Notation(BasicPlaying playing, int length);

    ~Notation();

    void display(int staff_x, int staff_y, int line, int col);

    static vector<Notation>
    generate_notation(Action action, Fraction play, Fraction offset, TimeSignature signature, BasicPlaying base);

private:
    MusicSymbol m_symbol;
};
