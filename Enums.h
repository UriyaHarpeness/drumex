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

    SymDownFlag = 'J',
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

// typedef pair<uint8_t, uint8_t> TimeSignature;
