#pragma once

#include <cstdint>
#include <vector>

using namespace std;

enum BasicPlaying {
    BasePlay,
    BaseStop,
};

enum Instrument {
    Unbound,
    ChinaInst,
    HiHatInst,
    HighTomInst,
    SnareInst,
    BassInst,
};

enum Playing {
    Stop,
    Regular,
    Flam,
    Ghost,
    Accent,
};

enum Sticking {
    UnboundStick,
    Right,
    Left,
};

enum MusicSymbolValues {
    SymZero = '0',
    SymOne = '1',
    SymTwo = '2',
    SymThree = '3',
    SymFour = '4',
    SymFive = '5',
    SymSix = '6',
    SymSeven = '7',
    SymEight = '8',
    SymNine = '9',

    SymSmallZero = 0xBC,
    SymSmallOne = 0xC1,
    SymSmallTwo = 0xAA,
    SymSmallThree = 0xA3,
    SymSmallFour = 0xA2,
    SymSmallFive = 0xB0,
    SymSmallSix = 0xA4,
    SymSmallSeven = 0xA6,
    SymSmallEight = 0xA5,
    SymSmallNine = 0xBB,

    SymDoubleWholeNote = 'W',
    SymWholeNote = 'w',
    SymHalfNote = 0xFA,
    SymQuarterNote = 0xCF,

    SymDownHalfNote = 0xFA,//'H',
    SymDownQuarterNote = 0xCF,//'Q',
    SymDownEightNote = 0xCF,//'E',
    SymDownSixteenthNote = 0xCF,//'X',
    SymDownFlag = 'J',

    SymUpHalfNote = 0xFA,//'h',
    SymUpEightNote = 0xCF,//'e',
    SymUpQuarterNote = 0xCF,//'q',
    SymUpSixteenthNote = 0xCF,//'x',
    SymUpFlag = 'j',

    SymDoubleWholeStop = 0xE3,
    SymWholeStop = 0xB7,
    SymHalfStop = 0xEE,
    SymQuarterStop = 0xCE,
    SymEightStop = 0xC5,
    SymSixteenthStop = 0xA8,
    SymThirtySecondStop = 0xF4,
    SymSixtyFourthStop = 0xE5,

    SymStaff = '=',
    SymClef = 0xD6,
    SymDot = 'k',
    SymCymbal = 0xC0,
    SymAccent = '>',
    SymGhost = '}',
    SymLedger = '_',
    SymTempo = 0xF2,
    SymBarLine = 0x5C,
};

enum MusicSymbol {
    DoubleWholeNote = 'W',
    WholeNote = 'w',
    HalfNote = 'H',
    QuarterNote = 'Q',
    EightNote = 'E',
    SixteenthNote = 'X',
    Flag = 'J',

    DoubleWholeStop = 0xE3,
    WholeStop = 0xB7,
    HalfStop = 0xEE,
    QuarterStop = 0xCE,
    EightStop = 0xC5,
    SixteenthStop = 0xA8,
    ThirtySecondStop = 0xF4,
    SixtyFourthStop = 0xE5,
};

enum Modifier {
    ModCrossStick,
    ModGhost,
    ModAccent,
    ModRimshot,
    ModFlam,
    ModDrag,
};

typedef tuple<Playing, Sticking> Action;
typedef vector<Action> Actions;
typedef pair<uint8_t, uint8_t> TimeSignature;
