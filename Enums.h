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

enum MusicSymbols {
    SymSmallZero = 0x80,
    SymSmallOne = 0x81,
    SymSmallTwo = 0x82,
    SymSmallThree = 0x83,
    SymSmallFour = 0x84,
    SymSmallFive = 0x85,
    SymSmallSix = 0x86,
    SymSmallSeven = 0x87,
    SymSmallEight = 0x88,
    SymSmallNine = 0x89,

    SymDoubleWholeNote = 'W',
    SymWholeNote = 'w',
    SymHalfNote = 0xFA,
    SymQuarterNote = 0xCF,

    SymDownFlag = 'J',
    SymUpFlag = 'j',

    SymDoubleWholeRest = 0xE3,
    SymWholeRest = 0xB7,
    SymHalfRest = 0xEE,
    SymQuarterRest = 0xCE,
    SymEightRest = 0xE4,
    SymSixteenthRest = 0xC5,
    SymThirtySecondRest = 0xA8,
    SymSixtyFourthRest = 0xF4,
    SymHundredTwentyEighthRest = 0xE5,

    SymStaff = '=',
    SymClef = 0xD6,
    SymDot = 'k',
    SymQuarterCymbal = 0xC0,
    SymWholeCymbal = 0xC3,
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
    SymDoubleBarLine = '[',
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

enum NotesDirection {
    NotesUp,
    NotesDown
};
