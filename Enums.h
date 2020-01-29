#pragma once

#include <cstdint>
#include <vector>

using namespace std;

enum BasicPlaying {
    BasePlay,
    BaseRest,
};

enum Instrument {
    Unbound,
    UnboundUp,
    UnboundDown,
    ChinaInst,
    CrashInst,
    HiHatInst,
    HighTomInst,
    MidTomInst,
    SnareInst,
    LowTomInst,
    FloorTomInst,
    BassInst,
};

enum Playing {
    Rest,
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

    SymDoubleWholeRest = 0xE3,
    SymWholeRest = 0xB7,
    SymHalfRest = 0xEE,
    SymQuarterRest = 0xCE,
    SymEightRest = 0xC5,
    SymSixteenthRest = 0xA8,
    SymThirtySecondRest = 0xF4,
    SymSixtyFourthRest = 0xE5,

    SymStaff = '=',
    SymClef = 0xD6,
    SymDot = 'k',
    SymCymbal = 0xC0,
    SymAccent = '>',
    SymGhost = '}',
    SymCrossStick = 'r',
    SymRimshot = 'p',
    SymFlam = 0xD9,
    SymDrag = 0xDA,
    SymClose = 0xF7,
    SymLoose = 0xF8,
    SymOpenClose = 0xF9,
    SymOpen = 0xF6,
    SymChoke = ',',
    SymRight = 'R',
    SymLeft = 'L',
    SymLedger = '_',
    SymTempo = 0xF2,
    SymBarLine = '\\',
};

enum MusicSymbol {
    DoubleWholeNote = 'W',
    WholeNote = 'w',
    HalfNote = 'H',
    QuarterNote = 'Q',
    EightNote = 'E',
    SixteenthNote = 'X',
    Flag = 'J',

    DoubleWholeRest = 0xE3,
    WholeRest = 0xB7,
    HalfRest = 0xEE,
    QuarterRest = 0xCE,
    EightRest = 0xC5,
    SixteenthRest = 0xA8,
    ThirtySecondRest = 0xF4,
    SixtyFourthRest = 0xE5,
};

enum Modifier {
    ModCrossStick,
    ModGhost,
    ModAccent,
    ModDot,
    ModRimshot,
    ModFlam,
    ModDrag,
    ModClose,
    ModLoose,
    ModOpenClose,
    ModOpen,
    ModChoke,
    ModRight,
    ModLeft,
};

typedef tuple<Playing, Sticking> Action;
typedef vector<Action> Actions;
typedef pair<uint8_t, uint8_t> TimeSignature;
